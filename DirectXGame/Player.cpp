#define NOMINMAX
#include <Player.h>
#include "Input.h"
#include "MapChipField.h"
#include "MathUtilityForText.h"
#include <DebugText.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <numbers>
#define _USE_MATH_DEFINES
#include <math.h>


void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	model_ = model;

	viewProjection_ = viewProjection;
}

void Player::Update() {
	// 回転角度（時間経過に応じて増加）
	static float angle = 0.0f;
	const float radius = 10.0f; // プレイヤーの回転半径
	const float speed = 0.01f;  // 回転速度

	// Z軸を中心に円を描くようにプレイヤー位置を更新
	worldTransform_.translation_.x = radius * cos(angle); // X座標を更新
	worldTransform_.translation_.y = radius * sin(angle); // Y座標は固定
	worldTransform_.translation_.z = radius * sin(angle); // Z座標を更新
	worldTransform_.rotation_.y += 0.02f;
	worldTransform_.rotation_.x += 0.02f;


	angle += speed;
	if (angle >= 2 * static_cast<float>(M_PI)) { // M_PIをfloatにキャスト
		angle -= 2 * static_cast<float>(M_PI);
	}

	// 行列を更新する
	worldTransform_.TransferMatrix();

	// プレイヤーの他の処理
	MovePlayer();
	// 衝突処理など他のロジック
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.movement = velocity_;
	collisionMapInfo.landingFlag = false;
	collisionMapInfo.wallContactFlag = false;
	CheckMapCollision(collisionMapInfo);
	JudgmentMove(collisionMapInfo);
	CeilingContact(collisionMapInfo);
	GraundSetting(collisionMapInfo);

	worldTransform_.UpdateMatarix();

	// 旋回処理
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= static_cast<float>(1.0f) / 60.0f;

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrdDirection_)];
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}
}


void Player::Draw() {

	model_->Draw(worldTransform_, *viewProjection_);
}

void Player::MovePlayer() {

	// 移動入力
	if (onGround_) {
		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				// 左移動中の右入力
				if (velocity_.x < 0.0f) {

					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
				if (lrdDirection_ != LRDirection::kRight) {
					lrdDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kLimitRunSpeed;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				// 右移動中の左入力
				if (velocity_.x > 0.0f) {

					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrdDirection_ != LRDirection::kLeft) {
					lrdDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kLimitRunSpeed;
				}
			}
			// 加速/減速
			velocity_.x += acceleration.x;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {

			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {

			// ジャンプ初速
			velocity_.y += kJumpAcceleration;
			// 空中
		}
	} else {
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}

void Player::CeilingContact(const CollisionMapInfo& info) {
	// 天井、当り判定
	if (info.hitCeilingFlag) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

void Player::CheckMapCollision(CollisionMapInfo& info) {

	CheckMapCollisionUp(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

void Player::CheckMapCollisionUp(CollisionMapInfo& info) {


	if (info.movement.y <= 0) {
		return;
	}


	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.movement, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	// 真上の当たり判定を行う
	bool hit = false;

	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);

	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {

		hit = true;
	}

	// ブロックにヒット
	if (hit) {

		MapChipField::IndexSet indexSetNow;

		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 3.0f, 0));

		if (indexSetNow.yIndex != indexSet.yIndex) {

			indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.movement + Vector3(0, +kHeight / 3.0f, 0));

			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.movement.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 3.0f + kBlank));
			info.hitCeilingFlag = true;
		}
	}
}

void Player::CheckMapCollisionDown(CollisionMapInfo& info) {

	if (info.movement.y >= 0) {
		return;
	}
	// 移動後４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + Vector3(0, info.movement.y, 0), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真上の当たり判定を行う
	bool hit = false;

	// 左下点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex) {
			// めり込みを排除する方向に移動量を設定する
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.movement + Vector3(0, -kHeight / 2.0f, 0));
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.movement.y = std::min(0.0f, (rect.top - worldTransform_.translation_.y) + ((kHeight / 2.0f) + kBlank));
			// 地面に当たったことを記録する
			info.landingFlag = true;
		}
	}
}

void Player::CheckMapCollisionRight(CollisionMapInfo& info) {

	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + Vector3(info.movement.x, 0, 0), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;

	// 左下点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 左上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.movement.x = std::max(0.0f, (rect.right - worldTransform_.translation_.x) - (kWidth / 2.0f + kBlank));
	}
}

void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {

	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + Vector3(info.movement.x, 0, 0), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;

	// 右下点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.movement.x = std::min(0.0f, (rect.left - worldTransform_.translation_.x) + (kWidth / 2.0f + kBlank));
	}
}

void Player::GraundSetting(const CollisionMapInfo& info) {
	// 接地状態の切り替え処理
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態の移行
			onGround_ = false;
		} else {
			std::array<Vector3, kNumCorner> positonsNew;

			for (uint32_t i = 0; i < positonsNew.size(); ++i) {
				positonsNew[i] = CornerPosition(worldTransform_.translation_ + info.movement, static_cast<Corner>(i));
			}

			MapChipType mapChipType;
			bool hit = false;

			// 左下点の判定
			MapChipField::IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positonsNew[kLeftBottom] + Vector3(0, 0, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 右下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positonsNew[kRightBottom] + Vector3(0, 0, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 落下開始
			if (!hit) {
				// 空中状態に切り替える
				onGround_ = false;
			}
		}
	} else {
		// 着地
		if (info.landingFlag) {
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuationLanding);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 着地状態に切り替える
			onGround_ = true;
		}
	}
}

void Player::JudgmentMove(const CollisionMapInfo& info) {
	// 移動
	worldTransform_.translation_ += info.movement;
}


Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, 
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, 
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

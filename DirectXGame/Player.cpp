#include "Player.h"
#include <cassert>
#include <Input.h>
#include <algorithm>


void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	assert(model);

	worldTransform_.Initialize();
	worldTransform_.translation_= position;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	model_ = model;
	//textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;

}



void Player::Update() {

	//移動入力
	//左右移動操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || 
		Input::GetInstance()->PushKey(DIK_LEFT)) {
	
		//左右加速
		Vector3 acceleration = {};

		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

			if (velocity_.x < 0.0f) {
			//逆方向にブレーキ
				velocity_.x *= (1.0f - kAtteunation);
			}
			acceleration.x += kAcceleration;

			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
			}

		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

			if (velocity_.x > 0.0f) {
			
			velocity_.x *= (1.0f - kAtteunation);

			}

		acceleration.x -= kAcceleration;

		if (lrDirection_ != LRDirection::kLeft) {
			lrDirection_ = LRDirection::kLeft;
		}
	} 
		//加速/減速
		velocity_.x += acceleration.x;
		velocity_.y += acceleration.y;
		velocity_.z += acceleration.z;

		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);


	} else {

		//左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.0f, 
			std::numbers::pi_v<float> * 3.0f / 2.0f
		};
		//状況に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		//自キャラの角度を設定する
		worldTransform_.rotation_.y = destinationRotationY;

		//入力していないときは減速させる
		velocity_.x *= (1.0f - kAtteunation);
		velocity_.y *= (1.0f - kAtteunation);
		velocity_.z *= (1.0f - kAtteunation);
	}
	
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	worldTransform_.UpdateMatarix();
}

void Player::Draw() {

	model_->Draw(worldTransform_,* viewProjection_, textureHandle_);

}

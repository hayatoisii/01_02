#include "Enemy2.h"
#include <numbers>

// 初期化
void Enemy2::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos) {
	model_ = model;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// プレイヤーの初期位置
	worldTransform_.translation_ = pos;
	// プレイヤーの初期角度
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	viewProjection_ = viewProjection;

	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer_ = 0.0f;
}

void Enemy2::Update() {

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

// 回転を制御する
	static float totalRotation = 0.0f;   // 現在の回転量
	static float rotationSpeed = 0.008f; // 回転速度
	static bool isRotatingRight = true;  // 右回転中かどうか

	// 右回転または中央に戻るチェック
	if (isRotatingRight) {
		// 右に回転（45度まで）
		RotateEnemy(rotationSpeed);
		totalRotation += rotationSpeed;

		// 45度以上回転したら反転して中央に戻る動作に切り替え
		if (totalRotation >= std::numbers::pi_v<float> / 4.0f) {
			isRotatingRight = false; // 中央に戻す動作に切り替え
		}
	} else {
		// 中央に戻る（回転量が0になるまで）
		RotateEnemy(-rotationSpeed);
		totalRotation -= rotationSpeed;

		// 回転量が0に戻ったら再び右回転に切り替える
		if (totalRotation <= 0.0f) {
			isRotatingRight = true; // 再び右回転に切り替え
		}
	}


	worldTransform_.UpdateMatarix();
}

// 描画
void Enemy2::Draw() {
	// 3D作成
	model_->Draw(worldTransform_, *viewProjection_);
}

Vector3 Enemy2::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Enemy2::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy2::OnCollision(const Player* player) { (void)player; }

// 例えば、回転速度や回転量を加算して回転を行う
void Enemy2::RotateEnemy(float rotationSpeed) {
	// 回転を加算する（Y軸回転）
	worldTransform_.rotation_.z += rotationSpeed;
}
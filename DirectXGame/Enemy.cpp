#include <Enemy.h>
#include <numbers>

// 初期化
void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos) 
{
	model_ = model;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// プレイヤーの初期位置
	worldTransform_.translation_ = pos;
	// プレイヤーの初期角度
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	viewProjection_ = viewProjection;

	velocity_ = {0, 0, -kWalkSpeed};
	walkTimer_ = 0.0f;
}


void Enemy::Update() { 
	
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	//worldTransform_.rotation_.y = -270.2f;
	worldTransform_.rotation_.y = 91.1f;

// 回転を制御する
	static float totalRotation = 0.0f;   // 現在の回転量
	static float rotationSpeed = 0.006f; // 回転速度
	static bool isRotatingRight = true;  // 右回転中かどうか

	// 右回転または左回転のチェック
	if (isRotatingRight) {
		// 右に回転（40度まで）
		RotateEnemy(rotationSpeed);
		totalRotation += rotationSpeed;

		// 40度以上回転したら左回転に切り替える
		if (totalRotation >= (34.0f * std::numbers::pi_v<float> / 180.0f)) {
			isRotatingRight = false; // 左回転に切り替え
		}
	} else {
		// 左に回転（40度まで）
		RotateEnemy(-rotationSpeed);
		totalRotation -= rotationSpeed;

		// -40度以上回転したら右回転に切り替える
		if (totalRotation <= -(34.0f * std::numbers::pi_v<float> / 180.0f)) {
			isRotatingRight = true; // 右回転に切り替え
		}
	}

	if (worldTransform_.translation_.z < -60) {
		worldTransform_.translation_.z = 70;
	}

	worldTransform_.UpdateMatarix(); 
}

// 描画
void Enemy::Draw() {
	// 3D作成
	model_->Draw(worldTransform_, *viewProjection_);
}

Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}

AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) { (void)player;}

// 例えば、回転速度や回転量を加算して回転を行う
void Enemy::RotateEnemy(float rotationSpeed) {
	// 回転を加算する（Y軸回転）
		worldTransform_.rotation_.x += rotationSpeed;
}


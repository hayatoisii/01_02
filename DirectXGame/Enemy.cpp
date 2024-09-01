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

	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer_ = 0.0f;
}

void Enemy::Update() { 
	
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;


	worldTransform_.translation_.x = 27.0f;
	worldTransform_.translation_.y = 10.0f;

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

void Enemy::OnCollision(const Player* player) 
{ 

	(void)player;

}


#define NOMINMAX
#include <ImGuiManager.h>
#include <Enemy.h>
#include <algorithm>
#include <cassert>

// 初期化
void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos) {
	assert(model);

	// 3Dモデルの作成
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// プレイヤーの初期位置
	worldTransform_.translation_ = pos;

	// プレイヤーの初期角度
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	viewProjection_ = viewProjection;
}

void Enemy::Update() { worldTransform_.UpdateMatarix(); }

// 描画
void Enemy::Draw() {
	// 3D作成
	model_->Draw(worldTransform_, *viewProjection_);
}
#define NOMINMAX
#include <ImGuiManager.h>
#include "MathUtilityForText.h"
#include <Enemy.h>
#include <algorithm>
#include <cassert>

// 初期化
void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos) 
{
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

	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer_ = 0.0f;
}

void Enemy::Update() { 
	
	worldTransform_.translation_ += velocity_;
	walkTimer_ += 1.0f / 60.0f;

	float parm = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMontionAngleEnd * (parm + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = radian;
	
	worldTransform_.UpdateMatarix(); 
}

// 描画
void Enemy::Draw() {
	// 3D作成
	model_->Draw(worldTransform_, *viewProjection_);
}
#include "Enemy.h"
#include "MathUtilityForText.h"
#include <cassert>
#include <numbers>

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;

	worldTranform_.Initialize();
	worldTranform_.translation_ = position;
	worldTranform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	viewProjection_ = viewProjection;

}

void Enemy::Update() {

}

void Enemy::Draw() { model_->Draw(worldTranform_, *viewProjection_); }
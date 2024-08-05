#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <numbers>
#include "Input.h"

// 自キャラ
class Enemy {
public:
	// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos);
	// 更新
	void Update();
	// 描画
	void Draw();

private:

	static inline const float kWalkSpeed = 0.05f;
	static inline const float kWalkMotionAngleStart = -1.0f;
	static inline const float kWalkMontionAngleEnd = 1.5f;
	static inline const float kWalkMotionTime = 1.0f;

	// 3Dモデル
	Model* model_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビューポート
	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {};
	float walkTimer_ = 0.0f;
};
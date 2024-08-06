#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Vector3SRT.h"
#include <algorithm>
#include <array>
#include "MathUtilityForText.h"

class DeathParticles {

public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

private:

	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;

	// パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransform_;

	// 消えるまでの時間
	static inline const float kDuration = 2.0f;
	// 移動
	static inline const float kSpeed = 0.05f;
	// 角度
	static inline const float kAngleUint = 2.0f * 3.14f / kNumParticles;
	// 終了フラグ
	bool isFinished_ = false;
	// カウント
	float counter_ = 0.0f;
	// 色変更オブジェクト
	ObjectColor objectColor_;
	// 色
	Vector4 color_;
};
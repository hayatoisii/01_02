#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "AABB.h"
#include "DebugText.h"

class MapChipField;
class Player;

class Enemy {
public:
	// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos);
	// 更新
	void Update();
	// 描画
	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// AABBを取得
	AABB GetAABB();

	void OnCollision(const Player* player);

	void RotateEnemy(float rotationSpeed);


private:

	// 3Dモデル
	Model* model_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	WorldTransform worldTransform2_;

	// ビューポート
	ViewProjection* viewProjection_ = nullptr;
	// マップチップ
	MapChipField* mapChipField_ = nullptr;
	// 速度
	Vector3 velocity_ = {};

	static inline const float kWalkSpeed = 0.05f;
	static inline const float kWalkMotionAngleStart = -1.0f;
	static inline const float kWalkMontionAngleEnd = 1.5f;
	static inline const float kWalkMotionTime = 1.0f;
	float walkTimer_ = 0.0f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

};
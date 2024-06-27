#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <numbers>
#include "Input.h"

enum class LRDirection {
	kRight,
	kLeft,
};

//自キャラ
class Player {
public:

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	~Player();

	private:

		//ワールド変換のデータ
		WorldTransform worldTransform_;
		//モデル
	    Model* model_ = nullptr;

		ViewProjection* viewProjection_ = nullptr;

		Vector3 velocity_ = {};

	    static inline const float kAcceleration = 0.1f;
	    static inline const float kAttenuation = 0.1f;
	    static inline const float kAttenuationLanding = 0.5f;

	    static inline const float kLimitRunSpeed = 1.0f;

		LRDirection lrDirection_ = LRDirection::kRight;

	    float turnFirstRotationY_ = 0.0f;
	    float turnTimer_ = 0.0f;

	    static inline const float kTimeTurn = 0.3f;
	    // 設置状態フラグ
	    bool onGround_ = true;
	    // 重力加速度(下)
	    static inline const float kGravityAcceleration = 0.98f;
	    // 最大落下速度(下)
	    static inline const float kLimitFallSpeed = 0.98f;
	    // ジャンプ初速(上)
	    static inline const float kJumpAcceleration = 5.0f;

};

#include "CameraController.h"
#include "Player.h"
#include <algorithm>
#include <iostream>

void CameraController::Initialize() { viewProjection_.Initialize(); }

void CameraController::Update() {
	if (!target_) {
		return;
	}

	const KamataEngine::WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	const KamataEngine::Vector3& targetVelocity = target_->GetVelocity();

	// Offset to move the player lower in the camera's view (e.g., -2.0f for lower placement)
	const float yOffset = -2.0f;

	// 追従対象とオフセットから目標座標を計算
	targetPosition_.x = targetWorldTransform.translation_.x + targetOffset_.x + targetVelocity.x * kVelocityBias;
	targetPosition_.y = targetWorldTransform.translation_.y + targetOffset_.y + targetVelocity.y * kVelocityBias + yOffset; // Apply yOffset
	targetPosition_.z = targetWorldTransform.translation_.z + targetOffset_.z + targetVelocity.z * kVelocityBias;

	// 追従対象が画面外に出ないように補正
	targetPosition_.x = (std::max)(targetPosition_.x, targetWorldTransform.translation_.x - margin_.left);
	targetPosition_.x = (std::min)(targetPosition_.x, targetWorldTransform.translation_.x + margin_.right);
	targetPosition_.y = (std::max)(targetPosition_.y, targetWorldTransform.translation_.y - margin_.bottom);
	targetPosition_.y = (std::min)(targetPosition_.y, targetWorldTransform.translation_.y + margin_.top);

	// 座標補間によりゆったり追従
	camera_.translation_ = Lerp(camera_.translation_, targetPosition_, kInterpolationRate);

	// 行列を更新する
	camera_.UpdateMatrix();
}


void CameraController::Reset() {
	if (!target_) {
		return;
	}
	viewProjection_.UpdateMatrix();
}
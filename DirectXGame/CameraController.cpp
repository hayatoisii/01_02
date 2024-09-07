#include "CameraController.h"
#include "Player.h"
#include <algorithm>
#include <iostream>

void CameraController::Initialize() { viewProjection_.Initialize(); }

void CameraController::Update() {
	if (!target_) {
		return;
	}

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// カメラの回転角度を設定
	viewProjection_.rotation_.y = 0.0f;
	viewProjection_.rotation_.x = -69.4f;

	// 追従対象とオフセットから目標座標を計算
	targetPosition_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	targetPosition_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	targetPosition_.z = targetWorldTransform.translation_.z + targetOffset_.z;

	// 追従対象が画面外に出ないように補正
	targetPosition_.x = (std::max)(targetPosition_.x, targetWorldTransform.translation_.x - margin_.left);
	targetPosition_.x = (std::min)(targetPosition_.x, targetWorldTransform.translation_.x + margin_.right);
	targetPosition_.y = (std::max)(targetPosition_.y, targetWorldTransform.translation_.y - margin_.bottom);
	targetPosition_.y = (std::min)(targetPosition_.y, targetWorldTransform.translation_.y + margin_.top);

	// カメラを即座に目標座標に追従させる（慣性を排除）
	viewProjection_.translation_ = targetPosition_;


	// 行列を更新する
	viewProjection_.UpdateMatrix();
	// デバッグ出力
	std::cerr << "Camera Position: (" << viewProjection_.translation_.x << ", " << viewProjection_.translation_.y << ", " << viewProjection_.translation_.z << ")" << std::endl;
}

void CameraController::Reset() {
	if (!target_) {
		return;
	}

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	viewProjection_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;

	viewProjection_.UpdateMatrix();
}

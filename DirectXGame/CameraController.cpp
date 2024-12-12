#include "CameraController.h"
#include "Player.h"
#include <algorithm>
#include <iostream>

void CameraController::Initialize() { viewProjection_.Initialize(); }

void CameraController::Update() {
	if (!target_) {
		return;
	}

	// 移動範囲制限
	viewProjection_.translation_.x = (std::max)(viewProjection_.translation_.x, movableArea_.left);
	viewProjection_.translation_.x = (std::min)(viewProjection_.translation_.x, movableArea_.right);
	viewProjection_.translation_.y = (std::max)(viewProjection_.translation_.y, movableArea_.bottom);
	viewProjection_.translation_.y = (std::min)(viewProjection_.translation_.y, movableArea_.top);

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

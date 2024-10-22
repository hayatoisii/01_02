#include "CameraController.h"
#include "Player.h"
#include <cmath> // sin, cos
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

void CameraController::Initialize() { viewProjection_.Initialize(); }

void CameraController::Update() {
	if (!target_) {
		return;
	}

	// 回転角度（時間経過に応じて増加）
	static float angle = 0.0f;
	const float radius = 10.0f; // カメラの回転半径
	const float speed = 0.01f;  // 回転速度

	// Z軸を中心に円を描くようにカメラ位置を更新
	viewProjection_.translation_.x = radius * cos(angle); // X座標を更新
	viewProjection_.translation_.y = 0.0f;                // Y座標は固定
	viewProjection_.translation_.z = radius * sin(angle); // Z座標を更新
	viewProjection_.rotation_.y += 0.01f;
	viewProjection_.rotation_.x += 0.01f;

	angle += speed;
	if (angle >= 2 * static_cast<float>(M_PI)) { // M_PIをfloatにキャスト
		angle -= 2 * static_cast<float>(M_PI);
	}

	// 行列を更新する
	viewProjection_.UpdateMatrix();
}

#pragma once
#include "MathUtilityForText.h"
#include "Vector3.h"
#include "ViewProjection.h"

class Player;

class CameraController {
public:
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	Player* target_ = nullptr;

	void Initialize();
	void Update();

	void setTarget(Player* target) { target_ = target; }

	const ViewProjection& GetViewProjection() const { return viewProjection_; }

private:
	ViewProjection viewProjection_;

};
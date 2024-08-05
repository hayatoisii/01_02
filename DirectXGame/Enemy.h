#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Enemy {

public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

private:

	Model* model_ = nullptr;
	WorldTransform worldTranform_;
	ViewProjection* viewProjection_ = nullptr;
	Vector3 velocity_ = {};
};
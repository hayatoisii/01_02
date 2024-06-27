#pragma once 
#include"Model.h"
#include "WorldTransform.h"

class Skydome {
public:
	void Initialize(Model* model, ViewProjection* viewProjection);

	void Update();

	void Draw();

private:
	WorldTransform S_worldTransform_;
	ViewProjection* S_viewProjection_ = nullptr;
	Model* S_model_ = nullptr;
	uint32_t S_textureHandle_ = 0u;
};

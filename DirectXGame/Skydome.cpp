#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);

	S_worldTransform_.Initialize();
	S_model_ = model;
	S_viewProjection_ = viewProjection;
}

void Skydome::Update() { S_worldTransform_.TransferMatrix(); }

void Skydome::Draw() {

	S_model_->Draw(S_worldTransform_, *S_viewProjection_);

}


#include "WorldTransform.h"
#include "Vector3SRT.h"
#include "4x4Matrixcalculation.h"


void WorldTransform::UpdateMatarix() {
	//スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	//定数バッファに転送する
	TransferMatrix();

}

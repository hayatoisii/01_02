#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <MakeMatrix.h>
#include <WorldTransform.h>
#include <Vector3SRT.h>


GameScene::GameScene() {}
// デストラクタ
GameScene::~GameScene() {

	delete model_;

	for (std::vector<WorldTransform*>& worldTransformBlockline : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockline) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {

	//texture_ = TextureManager::Load("cube.jpg");

	worldTransform_.Initialize();

	viewProjection_.Initialize();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 3Dモデルの生成02
	model_ = Model::Create();

	//要素数
	const uint32_t kNumBlockHorizontal = 20;

	//ブロック一個分の横幅
	const float kBlockWidth = 2.0f;

	//要素数を変更する
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	/*/
	//キューブの生成
	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) {
	
	worldTransformBlocks_[i] = new WorldTransform();
	worldTransformBlocks_[i]->Initialize();
	worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
	worldTransformBlocks_[i]->translation_.y = 0.0f;

	}
	/*/

	//要素数
	const uint32_t kNumBlockVirtical = 10;
	//const uint32_t kNumBlockHorizontal = 20;
	//ブロック1個分の横幅
	//const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	//要素数を変更する
	//列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
	//1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	//ブロックの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; i < kNumBlockHorizontal; ++i) {
		worldTransformBlocks_[i][j] = new WorldTransform();
		worldTransformBlocks_[i][j]->Initialize();
		worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
		worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}



}

void GameScene::Update() {

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		                                 
		worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

		worldTransformBlock->TransferMatrix();

	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			model_->Draw(*worldTransformBlock, viewProjection_); //,texture_
		}
    }


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

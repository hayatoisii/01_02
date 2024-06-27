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

	delete debugCamera_;
	delete player_;
	delete skydome_;
	delete mapChipField_;
	delete modelPlayer;
	delete Block_;
	delete modelSkydome_;

	for (std::vector<WorldTransform*>& worldTransformBlockline : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockline) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::GenerateBlocks() {

	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// １列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();


    Block_ = Model::Create();

	skydome_ = new Skydome();

	modelSkydome_ = Model::CreateFromOBJ("sky", true);

	skydome_->Initialize(modelSkydome_, &viewProjection_);

	viewProjection_.farZ = 1145;

	viewProjection_.Initialize();

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/map.csv");


	player_ = new Player;

	modelPlayer = Model::CreateFromOBJ("player", true);

	playerPosition_ = mapChipField_->GetMapChipPositionByIndex(10,10);

	player_ = new Player();
	player_->Initialize(modelPlayer, &viewProjection_, playerPosition_);

	GenerateBlocks();

}

void GameScene::Update() {


		debugCamera_->Update();

		#ifdef _DEBUG
	    if (input_->TriggerKey(DIK_K)) {
		    isDebugCamerActive_ = !isDebugCamerActive_;
		}
		#endif

		if (isDebugCamerActive_) {

		    viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		    viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		        
		        viewProjection_.TransferMatrix();
	    } else {
	    
		    viewProjection_.UpdateMatrix();
		}

		player_->Update();

		skydome_->Update();

	for (std::vector<WorldTransform*>& worldTransformBlockline : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockline) {
			if (!worldTransformBlock)
			    continue;
			    //worldTransformBlock->UpdateMatarix();
			    worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			    worldTransformBlock->TransferMatrix();
		    }
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
			if (!worldTransformBlock)
			continue;
			Block_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	player_->Draw();

	skydome_->Draw();

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

#pragma once

#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <MakeMatrix.h>
#include <WorldTransform.h>
#include <Vector3SRT.h>

GameScene::GameScene() {
	debugCamera_ = nullptr;
	player_ = nullptr;
	skydome = nullptr;
	mapChipField_ = nullptr;
	modelPlayer_ = nullptr;
	modelBlock_ = nullptr;
	modelSkydome_ = nullptr;
	cameraController_ = nullptr;
}

GameScene::~GameScene() {
	// ポインタの解放と nullptr 代入
	if (debugCamera_) {
		delete debugCamera_;
		debugCamera_ = nullptr;
	}

	if (player_) {
		delete player_;
		player_ = nullptr;
	}

	if (skydome) {
		delete skydome;
		skydome = nullptr;
	}

	if (mapChipField_) {
		delete mapChipField_;
		mapChipField_ = nullptr;
	}

	if (modelPlayer_) {
		delete modelPlayer_;
		modelPlayer_ = nullptr;
	}

	if (modelBlock_) {
		delete modelBlock_;
		modelBlock_ = nullptr;
	}

	if (modelSkydome_) {
		delete modelSkydome_;
		modelSkydome_ = nullptr;
	}

	if (cameraController_) {
		delete cameraController_;
		cameraController_ = nullptr;
	}

	// ブロックの解放
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
			worldTransformBlock = nullptr;
		}
		worldTransformBlockLine.clear();
	}
	worldTransformBlocks_.clear();
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定(縦方向のブロック数)
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を指定(横方向のブロック数)
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


	modelBlock_ = Model::CreateFromOBJ("block");

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// SkyDome作成
	skydome = new Skydome;

	modelSkydome_ = Model::CreateFromOBJ("sphere", true);

	skydome->Initialize(modelSkydome_, &viewProjection_);

	// Mapの生成
	mapChipField_ = new MapChipField;
	// Mapのよみこみ
	mapChipField_->LoadMapChipCsv("Resources/map.csv");

	modelPlayer_ = Model::CreateFromOBJ("player", true);

	modeltaitoru_ = Model::CreateFromOBJ("block", true);

	// 座標をマップチップ番号で指定
	playerPos = mapChipField_->GetMapChipPositionByIndex(9, 9);



		// プレイヤーの初期位置の取得
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(5, 18);
	Vector3 playerPosition2 = mapChipField_->GetMapChipPositionByIndex(5, 20);

		player_ = new Player();
		Player* player2_ = new Player;
		player_->SetMapChipField(mapChipField_);
		player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);
		player2_->Initialize(modelPlayer_, &viewProjection_, playerPosition2);

	debugCamera_ = new DebugCamera(1280, 720);

	GenerateBlocks();

	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->setTarget(player_);
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);
	cameraController_->Reset();
}


void GameScene::Update() {

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

	} else {
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			worldTransformBlock->UpdateMatarix();
			worldTransformBlock->TransferMatrix();
		}
	}


		player_->Update();
    

	skydome->Update();

	cameraController_->Update();

	viewProjection_.TransferMatrix();

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
	///

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
	///

	player_->Draw();
	
	skydome->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
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
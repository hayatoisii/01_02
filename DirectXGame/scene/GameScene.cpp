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
	delete debugCamera_;
	delete player_;
	delete skydome;
	delete mapChipField_;
	delete modelPlayer_;
	delete modelBlock_;
	delete modelSkydome_;
	delete cameraController_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
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


	// 自キャラの生成
	player_ = new Player;

	modelPlayer_ = Model::CreateFromOBJ("player", true);

	// 座標をマップチップ番号で指定
	playerPos = mapChipField_->GetMapChipPositionByIndex(9, 9);

	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &viewProjection_, playerPos);

		// プレイヤーの初期位置の取得
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(5, 18);

	// プレイヤーの生成と初期化
	player_ = new Player();
	player_->SetMapChipField(mapChipField_);
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);

	debugCamera_ = new DebugCamera(1280, 720);

	GenerateBlocks();

	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->setTarget(player_);
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
}


void GameScene::Update() {

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	if (isDebugCameraActive_) {
		debugCamera_->Update();


	} else {
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
		playerPos.x = ;
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			worldTransformBlock->UpdateMatarix();
			worldTransformBlock->TransferMatrix();
		}
	}

	// 自キャラの更新
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


	// 自キャラ
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
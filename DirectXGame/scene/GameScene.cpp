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

	// delete modelBlock_
	delete model_;;
	delete debugCamera_;
	delete sprite_;
	delete player_;

	for (std::vector<WorldTransform*>& worldTransformBlockline : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockline) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete modelSkydome_;

	delete mapChipField_;

}

void GameScene::Initialize() {

	// ファイル名を指定してテクスチャを読み込む、02
	textureHandle_ = TextureManager::Load("sample.png");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	// ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	worldTransform_.Initialize();

	viewProjection_.Initialize();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 3Dモデルの生成02
	model_ = Model::Create();

	skydome_ = new Skydome();

	modelSkydome_ = Model::CreateFromOBJ("sky", true);

	skydome_->Initialize(modelSkydome_, &viewProjection_);


	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/map.csv");

	GenerateBlocks();


	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(10,10);

	player_ = new Player();
	player_->Initialize(model_, &viewProjection_, playerPosition);



}

void GameScene::Update() {

		for (std::vector<WorldTransform*>& worldTransformBlockline : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockline) {
			    if (!worldTransformBlock)
				continue;
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				worldTransformBlock->TransferMatrix();
			}
		}


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

	for (std::vector<WorldTransform*>& worldTransformBlockline : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockline) {
			if (!worldTransformBlock)
			continue;

			model_->Draw(*worldTransformBlock, viewProjection_); 
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

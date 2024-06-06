#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include <vector>
#include "Skydome.h"
#include "MapChipField.h"
#include <Player.h>



/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	void GenerateBlocks();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライトの生成
	Sprite* sprite_ = nullptr;

	// 3Dモデル02
	Model* model_ = nullptr;
	                            

	// ワールドトランスフォーム
    WorldTransform worldTransform_;

	ViewProjection viewProjection_;


	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;


	bool isDebugCamerActive_ = false;

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	//天球
	Skydome* skydome_ = nullptr;
	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//マップチップフィールド
	MapChipField* mapChipField_;

	// 自キャラ
	Player* player_ = nullptr;
	
	//numBlockHorizontal横方向の個数

	/// </summary>
};

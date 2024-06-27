#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "Skydome.h"
#include <DebugCamera.h>
#include "Player.h"
#include<vector>
#include<MapChipField.h>



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

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	ViewProjection viewProjection_;


	bool isDebugCamerActive_ = false;
	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	//天球
	Skydome* skydome_ = nullptr;
	//3Dモデル
	Model* modelPlayer = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* Block_ = nullptr;
	// 自キャラ
	Player* player_ = nullptr;
	Vector3 playerPosition_ = {};

	//マップチップフィールド
	MapChipField* mapChipField_;
	
	//numBlockHorizontal横方向の個数

	/// </summary>
};

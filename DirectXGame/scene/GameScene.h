#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Skydome.h"
#include "Player.h"
#include "DebugCamera.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "Enemy.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	

	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Vector3 playerPos = {};

	Skydome* skydome = nullptr;

	MapChipField* mapChipField_ = nullptr;

	Model* modelPlayer_ = nullptr;

	Model* modelBlock_ = nullptr;

	Model* modelSkydome_ = nullptr;

	Model* modelEnemy_ = nullptr;

	CameraController* cameraController_;

	// ワールドビュー
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;


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
	/// <summary>
	/// 表示ブロックの生成
	/// </summary>
	void GenerateBlocks();

private: // メンバ変数

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

};

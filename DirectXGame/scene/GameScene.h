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
#include "AABB.h"
#include "DeathParticles.h"

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
	Skydome* skydome = nullptr;
	MapChipField* mapChipField_ = nullptr;
	DeathParticles* dethParticles_ = nullptr;

	Model* modelEnemy_ = nullptr;
	Model* modelPlayer_ = nullptr;
	Model* modelBlock_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelParticles_ = nullptr;

	Model* modelWater_ = nullptr;
	Model* modelLava_ = nullptr;
	Model* modelGrass_ = nullptr;
	Model* modelSand_ = nullptr;
	Model* modelEnemy1_ = nullptr;
	Model* modelGoal_ = nullptr;
	Model* modelNewType_ = nullptr; // 新しいモデル

	float kBlockWidth;
	float kBlockHeight;

	Vector3 playerPos = {};

	CameraController* cameraController_;

	void CheckAllCollisions();

	std::list<Enemy*> enemies_;

	// ワールドビュー
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// ゲームのフェーズ(型)
	enum class Phase {
		kPlay,  // ゲームプレイ
		kDeath, // デス演出
	};

	Phase phace_;
	bool finished_ = false;

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

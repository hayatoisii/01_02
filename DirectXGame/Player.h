#pragma once
#include "Model.h"
#include "WorldTransform.h"

//自キャラ
class Player {
public:
	/// <summarty>
	///	初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);

	/// <summary>
	///　更新
	///  </summary>
	void Update();

	/// <summary>
	/// 描画　
	/// </summary>
	void Draw();

	private:

		//ワールド変換のデータ
		WorldTransform worldTransform_;
		//モデル
	    Model* model_ = nullptr;
		//テクスチャハンドル
	    uint32_t textureHandle_ = 0u;

		ViewProjection* viewProjection_ = nullptr;


};

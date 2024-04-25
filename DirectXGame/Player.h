#pragma once
#include "Model.h"
#include "WorldTransform.h"

//自キャラ
class Player {
public:
	/// <summarty>
	///	初期化
	/// </summary>
	void Initialize();

	/// <summary>
	///　更新
	///  </summary>
	void Update();

	/// <summary>
	/// 描画　
	/// </summary>
	void Draw();

};

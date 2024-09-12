#pragma once
#include "Sprite.h"

class Record final{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Record() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Record() = default;


public:

	/// <summary>
	/// スコアをリセット
	/// </summary>
	void ResetScore();

	/// <summary>
	/// スコアを加算
	/// </summary>
	void AddScore();



private:
	//合計スコア
	int32_t totalScore_ = 0;

};


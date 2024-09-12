#pragma once

#include <memory>
#include "Sprite.h"


class GameScoreUI{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScoreUI() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();



	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScoreUI() = default;


public:
	/// <summary>
	/// スコアの設定
	/// </summary>
	/// <param name="score"></param>
	void SetScore(int32_t& score) {
		this->score_ = score;
	}


	/// <summary>
	/// 表示させるかどうかの設定
	/// </summary>
	/// <param name="isDisplay"></param>
	void SetIsDisplay(bool isDisplay) {
		this->isDisplay_ = isDisplay;
	}




private:
	//スコア
	int32_t score_ = 0;

	//表示させるかどうか
	bool isDisplay_ = false;



};


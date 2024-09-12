#pragma once

#include <memory>
#include <array>
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

	//5桁が最大
	static const uint32_t DIGIT_ = 5u;
	std::array<std::unique_ptr<Sprite>, DIGIT_>sprites_ = {nullptr};

	enum NumberPlace {
		//一
		OnePlace,
		//十
		TenPlace,
		//百
		HundredPlace,
		//千
		ThousandPlace,
		//万
		TenThousandPlace,
	};

	//位
	int32_t places_[DIGIT_] = {};
	static const uint32_t NUMBER_QUANTITY = 10;
	uint32_t numberQuantity[NUMBER_QUANTITY] = {};

};


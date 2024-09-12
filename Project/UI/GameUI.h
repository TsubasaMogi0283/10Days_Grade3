#pragma once

#include <memory>
#include <array>
#include <memory>

#include "Sprite.h"


class GameUI{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameUI() = default;

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
	~GameUI() = default;


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


	/// <summary>
	/// 時間取得
	/// </summary>
	/// <returns></returns>
	int32_t GetTime()const {
		return time_;
	}


	/// <summary>
	/// //始めるかどうか
	/// </summary>
	/// <param name="isStart"></param>
	void SetIsTimeStart(bool isStart) {
		this->isTimeStart_ = isStart;
	}


private:
	//スコア
	int32_t score_ = 0;

	//表示させるかどうか
	bool isDisplay_ = false;

	//5桁が最大
	static const uint32_t SCORE_DIGIT_ = 5u;
	std::array<std::unique_ptr<Sprite>, SCORE_DIGIT_>scoreSprites_ = {nullptr};

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
	int32_t scorePlaces_[SCORE_DIGIT_] = {};
	static const uint32_t NUMBER_QUANTITY = 10;
	//スプライト
	uint32_t numberQuantity[NUMBER_QUANTITY] = {};





	//時間
	int32_t time_ = 0;
	const int32_t SECOND = 60;
	//現在の時間
	int32_t currentTime_ = 60;
	//始めるかどうか
	bool isTimeStart_ = false;

	//時間が過ぎたかどうか
	bool isTimeOver_ = false;

	//桁
	static const uint32_t TIME_DIGIT_ = 2u;
	//位
	int32_t timePlaces_[TIME_DIGIT_] = {};
	//スプライト
	std::array<std::unique_ptr<Sprite>, TIME_DIGIT_>timeSprites_ = { nullptr };


};


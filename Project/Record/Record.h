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

	//インスタンス
	static Record* GetInstance();

	//コピーコンストラクタ禁止
	Record(const Record& record) = delete;

	//代入演算子を無効にする
	Record& operator=(const Record& record) = delete;



public:

	/// <summary>
	/// スコアをリセット
	/// </summary>
	void ResetScore();

	/// <summary>
	/// スコアを加算
	/// </summary>
	void AddRockScore();

	/// <summary>
	/// 鉄のスコア
	/// </summary>
	void AddFeScore();

	/// <summary>
	/// 減点
	/// </summary>
	void DeductionScore();

	/// <summary>
	/// 合計スコアを取得
	/// </summary>
	/// <returns></returns>
	inline int32_t GetTotalScore() const{
		return totalScore_;
	}

private:
	//合計スコア
	int32_t totalScore_ = 0;

	//岩のスコア
	const int32_t ROCK_POINT_ = 100;
	//鉄のスコア
	const int32_t FE_POINT_ = 200;
	//減点
	const int32_t DEDUCATION_POINT_ = -100;




};


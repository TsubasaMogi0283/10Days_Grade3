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
	/// 更新
	/// </summary>
	void Update();


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




public:

	/// <summary>
	/// 一増やす
	/// </summary>
	void AddOnePlace();

	/// <summary>
	/// 十増やす
	/// </summary>
	void AddTenPlace();

	/// <summary>
	/// 百増やす
	/// </summary>
	void AddHundredPlace();

	/// <summary>
	/// 千増やす
	/// </summary>
	void AddThousandPlace();

	/// <summary>
	/// 万増やす
	/// </summary>
	void AddTenThousandPlace();


	void SetNextSceneFromResult(int32_t next) {
		this->nextSceneFromResult_ = next;
	}

	int32_t GetNextSceneFromResult() {
		return nextSceneFromResult_;
	}


private:
	//合計スコア
	int32_t totalScore_ = 0;

	//岩のスコア
	const int32_t ROCK_POINT_ = 100;
	//鉄のスコア
	const int32_t FE_POINT_ = 200;
	//減点
	const int32_t DEDUCATION_POINT_ = -50;




private:
	int32_t nextSceneFromResult_ = -1;
};


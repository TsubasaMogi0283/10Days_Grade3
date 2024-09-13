#pragma once

#include "Audio.h"


// プレイヤー関連のサウンドハンドル
struct PlayerSEType {
	uint32_t Jump;
	uint32_t Stomp;
	uint32_t Damaged;
};


/* プレイヤーのSE関連クラス */
class PlayerSE {

public:

	// コンストラクタ、デストラクタ
	PlayerSE();
	~PlayerSE() = default;

	// サウンド再生
	void PlaySE(uint32_t seType);


private:

	// オーディオクラス
	Audio* se_ = nullptr;
};


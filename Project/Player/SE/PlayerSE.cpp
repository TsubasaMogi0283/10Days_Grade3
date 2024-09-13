#include "PlayerSE.h"



// コンストラクタ
PlayerSE::PlayerSE()
{
	this->se_ = Audio::GetInstance();
}

// サウンド再生
void PlayerSE::PlaySE(uint32_t seType)
{
	se_->PlayWave(seType, false);
}


#include "GameAudioScene.h"

void GameAudioScene::Initialize(){
	bgm_ = Audio::GetInstance();
	bgmHandle = bgm_->LoadWave("Resources/Audio/Game/Game.wav");
	bgm_->PlayWave(bgmHandle, true);
	bgm_->ChangeVolume(bgmHandle,0.5f);
}

void GameAudioScene::Update(){

}

GameAudioScene::~GameAudioScene(){
	bgm_->StopWave(bgmHandle);
}

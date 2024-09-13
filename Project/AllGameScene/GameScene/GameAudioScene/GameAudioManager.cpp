#include "GameAudioManager.h"

void GameAudioManager::Initialize()
{
}

void GameAudioManager::Update()
{
}

void GameAudioManager::ChangeGameAudioScene(IGameAudioScene* iGameAudioScene){
	//一度消してから次のシーンにいく
	delete currentScene_;
	currentScene_ = iGameAudioScene;
	//引数が今入っているシーン
	currentScene_->Initialize();
}

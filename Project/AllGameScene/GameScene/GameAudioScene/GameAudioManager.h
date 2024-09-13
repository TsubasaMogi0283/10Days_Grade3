#pragma once
#include "IGameAudioScene.h"

class GameAudioManager{
public:
	GameAudioManager() = default;


	void Initialize();


	void Update();


	void ChangeGameAudioScene(IGameAudioScene* iGameAudioScene);


	~GameAudioManager() = default;






private:


	IGameAudioScene* currentScene_ = nullptr;

};


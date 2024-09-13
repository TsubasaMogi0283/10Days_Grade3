#pragma once
#include "IGameAudioScene.h"

class GameAudioSceneFinish:public IGameAudioScene {
public:
	GameAudioSceneFinish() = default;

	void Initialize() override;

	void Update() override;



	~GameAudioSceneFinish();



private:


};


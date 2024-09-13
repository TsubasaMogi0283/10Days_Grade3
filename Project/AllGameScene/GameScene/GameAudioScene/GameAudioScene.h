#pragma once
#include "iGameAudioScene.h"
#include <Audio.h>

class GameAudioScene :public IGameAudioScene{
public:
	GameAudioScene() = default;

	void Initialize() override;

	void Update() override;



	~GameAudioScene();

private:
	Audio* bgm_ = nullptr;
	uint32_t bgmHandle = 0u;
};


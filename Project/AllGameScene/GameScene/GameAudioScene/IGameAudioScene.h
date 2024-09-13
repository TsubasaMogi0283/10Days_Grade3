#pragma once
#include <cstdint>

class IGameAudioScene {
public:
	virtual void Initialize() = 0;

	virtual void Update() = 0;



	virtual ~IGameAudioScene()=default;

protected:
	int32_t audioState_ = -1;

};

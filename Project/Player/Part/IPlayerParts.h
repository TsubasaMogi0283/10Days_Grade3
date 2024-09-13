#pragma once

#include <list>
#include <memory>

#include "WorldTransform.h"


// PlayerPartのタイプ
enum class PlayerPartType {
	HEAD,
	TORSO,
	Count,
};

// 前方宣言
struct Camera;
struct DirectionalLight;
class Player;


/* PlayerPartの基底クラス */
class PlayerParts {

public:

	// 仮想デストラクタ
	virtual ~PlayerParts() = default;

	// ペアレントを結ぶ : 純粋仮想関数
	virtual void SetParent(WorldTransform* parent) = 0;

};
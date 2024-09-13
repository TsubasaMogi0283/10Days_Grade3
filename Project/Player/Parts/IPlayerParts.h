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
class IPlayerParts {

public:

	// 仮想デストラクタ
	virtual ~IPlayerParts() = default;

	// 初期化、更新、描画
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw3D(Camera& camera, DirectionalLight& light) = 0;

	// ペアレントを結ぶ : 純粋仮想関数
	virtual void SetParent(WorldTransform* parent) = 0;

};
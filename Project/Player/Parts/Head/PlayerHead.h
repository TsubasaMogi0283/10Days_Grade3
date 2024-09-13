#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"

#include "../IPlayerParts.h"


/* Playerの頭 */
class PlayerHead : public IPlayerParts {

public:

	// コンストラクタ、デストラクタ
	PlayerHead() = default;
	~PlayerHead() = default;

	// コピーコンストラクタ
	PlayerHead(uint32_t handle);

	// 初期化、更新、描画
	void Init() override;
	void Update() override;
	void Draw3D(Camera& camera, DirectionalLight& light) override;


#pragma region Accessor アクセッサ

	// ペアレントを結ぶ
	void SetParent(WorldTransform* parent) override { this->transform_.parent_ = parent; }

#pragma endregion 


private:

	// モデル
	uint32_t modelHandle_ = 0;
	std::unique_ptr<Model> model_;
	// トランスフォーム
	WorldTransform transform_{};
	// マテリアル
	Material mtl_{};
};


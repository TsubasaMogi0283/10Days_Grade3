#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"


// 前方宣言
class Input;
class AdjustmentItems;
struct Camera;
struct DirectionalLight;


/* Playerクラス */
class Player {

public:

	// コンストラクタ、デストラクタ
	Player() = default;
	~Player() = default;

	// コピーコンストラクタ
	Player(uint32_t modelHandle);

	// 初期化、更新、描画
	void Init();
	void Update();
	void Draw3D(Camera& camera, DirectionalLight& light);


private:

	// Imguiの描画
	void DrawImGui();


private:

	// モデル
	uint32_t modelHandle_ = 0;
	std::unique_ptr<Model> model_;
	// トランスフォーム
	WorldTransform transform_{};
	// マテリアル
	Material mtl_{};


#pragma region System システム

	// 入力
	Input* input_ = nullptr;

	// グローバルバリューズ
	AdjustmentItems* itemManager_ = nullptr;


#pragma endregion 

};


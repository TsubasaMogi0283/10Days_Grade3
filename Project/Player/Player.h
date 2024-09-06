#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "Input.h"
#include "VectorCalculation.h"


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

	// 移動処理
	void Move(XINPUT_STATE joyState);


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

	// 移動量
	Vector3 velocity_{};
	// 移動速度
	float moveSpeed_ = 0.3f;


#pragma region System システム


#pragma endregion 

};


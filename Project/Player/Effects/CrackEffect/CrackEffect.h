#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "VectorCalculation.h"
#include "Matrix4x4Calculation.h"
#include "../../Func/PlayerFunc.h"


/* プレイヤーのストンプの亀裂のエフェクト */
class CrackEffect {

public:

	// コンストラクタ、デストラクタ
	CrackEffect() = default;
	~CrackEffect() = default;

	// コピーコンストラクタ
	CrackEffect(uint32_t handle);

	// 初期化、更新、描画
	void Init();
	void Update();
	void Draw3D(Camera& camera, DirectionalLight& light);


#pragma region Accessor アクセッサ


#pragma endregion 


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

	// 出現させるか
	bool isActive_ = false;

};


#pragma once

#include <random>

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "VectorCalculation.h"
#include "Matrix4x4Calculation.h"
#include "../../Func/PlayerFunc.h"
#include "../../Timer/pTimer.h"
#include "../../Ease/Ease.h"


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

	// アクティブ
	bool IsActive() const { return this->isActive_; }

	// スケール
	void SetScale(Vector3 setScale) { this->transform_.scale_ = setScale; }

	// Y軸姿勢
	void SetYRotate(float setRotate) { this->transform_.rotate_.y = setRotate; }

	// 座標
	void SetPosition(Vector3 setPos) { this->transform_.translate_ = setPos; }

#pragma endregion 


private:

	// alpha値を減らす処理
	bool SubAlpha();

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

	// アクティブ
	bool isActive_ = false;

	// 寿命
	pTimer timer_{};

	// 乱数生成器
	std::mt19937 randomEngine_;
};


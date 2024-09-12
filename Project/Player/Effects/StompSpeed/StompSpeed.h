#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "VectorCalculation.h"
#include "Matrix4x4Calculation.h"
#include "../../Func/PlayerFunc.h"


namespace pEffect {

	/* ストンプのスピード感を出すエフェクト */
	class StompSpeed {

	public: 

		// コンストラクタ、デストラクタ
		StompSpeed() = default;
		~StompSpeed() = default;

		// コピーコンストラクタ
		StompSpeed(uint32_t handle);

		// 初期化、更新、描画
		void Init();
		void Update();
		void Draw3D(Camera& camera, DirectionalLight& light);


#pragma region Accessor アクセッサ


#pragma endregion 


	private:

		// 乱数回転処理
		void RandOrientation();

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
}
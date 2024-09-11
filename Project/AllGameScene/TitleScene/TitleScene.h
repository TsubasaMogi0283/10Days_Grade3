#pragma once
#include "IGameScene.h"

#include <memory>

#include "Sprite.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Camera.h"
#include <BackText.h>

//StatePatternを使う時は必ず前方宣言をするように
class GameManager;

class TitleScene : public IGameScene {
public:

	//コンストラクタ
	TitleScene()=default;


	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

#pragma region 描画

	/// <summary>
	/// ポストエフェクト掛ける前のスプライト
	/// </summary>
	void DrawSpriteBack()override;


	/// <summary>
	/// 3Dオブジェクト
	/// </summary>
	void DrawObject3D()override;


	void PreDrawPostEffectFirst()override;
	void DrawPostEffect()override;


	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

#pragma endregion
	/// デストラクタ
	~TitleScene();




private:
	ModelManager* modelManager_ = nullptr;
	ModelManager* modelManager2_ = nullptr;
	//カメラ
	Camera camera_ = {};
	Vector3 cameraPosition_ = {};
	DirectionalLight directtionalLight_ = {};
	//マテリアル
	Material material_ = {};

	//ポストエフェクト
	//今は使わない
	std::unique_ptr<BackText> back_ = nullptr;

	//タイトル
	uint32_t modelHandle_ = 0;
	std::unique_ptr<Model> titleModel_ = nullptr;
	Material mtl_{};
	WorldTransform transform_{};
	//タイトル2
	uint32_t modelHandle2_ = 0;
	std::unique_ptr<Model> titleModel2_ = nullptr;
	Material mtl2_{};
	WorldTransform transform2_{};

};


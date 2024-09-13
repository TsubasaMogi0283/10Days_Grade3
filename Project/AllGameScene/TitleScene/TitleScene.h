#pragma once
#include "IGameScene.h"


#include <memory>

#include "Sprite.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Camera.h"
#include <BackText.h>
#include "../External/TsumiInput/TInput.h"
#include "Audio.h"

//StatePatternを使う時は必ず前方宣言をするように
class GameManager;

class TitleScene : public IGameScene {
public:

	//コンストラクタ
	TitleScene() = default;


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


	TInput* tInput_ = nullptr;
	//カメラ
	Camera camera_ = {};
	Vector3 cameraPosition_ = {};
	DirectionalLight directtionalLight_ = {};
	//マテリアル
	Material material_ = {};

	//ポストエフェクト
	//今は使わない
	std::unique_ptr<BackText> back_ = nullptr;
	///フラグ
	bool isPlayScene_;
	float speed;
	//タイトル
	uint32_t modelHandle_ = 0;
	std::unique_ptr<Model> titleModel_ = nullptr;
	Material mtl_{};
	WorldTransform transform_{};
	ModelManager* modelManager_ = nullptr;
	//タイトル2
	uint32_t modelHandle2_ = 0;
	std::unique_ptr<Model> titleModel2_ = nullptr;
	Material mtl2_{};
	WorldTransform transform2_{};
	ModelManager* modelManager2_ = nullptr;
	//B
	uint32_t modelHandleB_ = 0;
	std::unique_ptr<Model> titleModelB_ = nullptr;
	Material mtlB_{};
	WorldTransform transformB_{};
	ModelManager* modelManagerB_ = nullptr;
	//
	std::unique_ptr<Model> model_;
	std::unique_ptr<Model>drill_ = nullptr;
	Material mtlD_{};
	WorldTransform transformD_{};
	ModelManager* modelManagerD_ = nullptr;
	//
	uint32_t modelHandleDungeon_ = 0;
	std::unique_ptr<Model> titleModelDungeon_ = nullptr;
	Material mtlDungeon_{};
	WorldTransform transformDungeon_{};
	ModelManager* modelManagerDungeon_ = nullptr;

	float theta_ = 0.0f;

	std::unique_ptr<Sprite> white_ = nullptr;
	float whiteAlpha_ = 0.0f;
	bool isFadeOut_ = false;

	Audio* bgm_ = nullptr;
	float filter_ = 1.0f;
	uint32_t bgmHandle = 0u;

	Audio* startSE_ = nullptr;
	uint32_t seHandle_ = 0u;
	int32_t startSETime_ = 0;
};

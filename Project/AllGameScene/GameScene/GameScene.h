#pragma once
#include "IGameScene.h"

#include <memory>

#include "Sprite.h"

#include "Model.h"
#include "Camera.h"
#include <BackText.h>


#include "WorldTransform.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "SpotLight.h"


#include "Player/Player.h"




//StatePatternを使う時は必ず前方宣言をするように
class GameManager;
class ModelManager;
class Input;

class GameScene : public IGameScene {
public:

	//コンストラクタ
	GameScene();


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
	~GameScene();



private:

	/// <summary>
	/// プレイヤーの移動処理
	/// </summary>
	void PlayerMove();




private:

	//カメラ
	Camera camera_ = {};
	Vector3 cameraPosition_ = {};


	//ポストエフェクト
	//今は使わない
	std::unique_ptr<BackText> back_ = nullptr;

	//平行光源
	DirectionalLight directtionalLight_ = {};


	SpotLight light_ = {};


	/* ----- Player プレイヤー ----- */
	// 本体
	std::unique_ptr<Player> playe_;


#pragma region System

	// モデルマネージャー
	ModelManager* modelManager_ = nullptr;

	// 入力
	Input* input_ = nullptr;

	//操作は全部ゲームシーンで統一させたい
	//コマンドパターンですっきりさせても良さそう
	XINPUT_STATE joyState_{};


#pragma endregion 
};



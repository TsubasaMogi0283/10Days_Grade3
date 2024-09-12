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
#include <StageObject/Ground/Groud.h>
#include "Enemy/RockEnemy.h"


#include "FollowCamera/FollowCamera.h"
#include "Player/Player.h"
#include <Enemy/EnemyManager.h>
#include <Collider/CollisionManager.h>




//StatePatternを使う時は必ず前方宣言をするように
class GameManager;
class ModelManager;
class Input;
class TInput;

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
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();



private:

	/// <summary>
	/// 入力関連処理
	/// </summary>
	void FuncInput();




private:

	//カメラ
	Camera camera_ = {};
	Vector3 cameraPosition_ = {};

	//平行光源
	DirectionalLight directtionalLight_ = {};



	//ポストエフェクト
	//今は使わない
	std::unique_ptr<BackText> back_ = nullptr;


	/* ----- FollowCamera フォローカメラ ----- */
	std::unique_ptr<FollowCamera> followCamera_;

	/* ----- Player プレイヤー ----- */
	std::unique_ptr<Player> player_;


#pragma region System

	// モデルマネージャー
	ModelManager* modelManager_ = nullptr;

	// 入力
	Input* input_ = nullptr;
	TInput* tInput_ = nullptr;

	//操作は全部ゲームシーンで統一させたい
	//コマンドパターンですっきりさせても良さそう
	XINPUT_STATE joyState_{};


#pragma endregion 

	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;


	//敵管理クラス
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;



	//地面
	std::unique_ptr<Ground>ground_ = nullptr;


};



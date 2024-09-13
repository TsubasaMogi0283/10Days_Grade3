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
#include "Record/Record.h"

#include "FollowCamera/FollowCamera.h"
#include "Player/Player.h"
#include <Enemy/EnemyManager.h>
#include <Collider/CollisionManager.h>
#include "UI/GameUI.h"
#include <StageObject/Skydome/Skydome.h>
#include "GameAudioScene/GameAudioManager.h"

#include "Audio.h"


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

	/// <summary>
	/// あらかじめ読み込んでおく
	/// </summary>
	void PreLoad();

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

	//記録
	Record* record_ = nullptr;

#pragma endregion 

	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;


	//敵管理クラス
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;



	//地面
	std::unique_ptr<Ground>ground_ = nullptr;


	std::unique_ptr<Skydome>skydome_ = nullptr;



private:
	//UI
	std::unique_ptr<GameUI> gameUI_ = nullptr;
	





	std::unique_ptr<Sprite> white_ = nullptr;
	float whiteAlpha_ = 1.0f;

	std::unique_ptr<Sprite> startSprite_ = nullptr;
	std::unique_ptr<Sprite> endSprite_ = nullptr;



	std::unique_ptr<GameAudioManager> gameAudioManager_ = nullptr;
	int32_t gameChange_ = 0;
private:

	enum GameCondition {
		FadeIn,
		CountDown,
		GamePlay,
		FinishGame,
		FadeOut,
	};

	int32_t condition_ = -1;

	bool isFadeIn_ = true;
	bool isGamePlay_ = false;
	bool isFinishGame_ = false;
	bool isFadeOut = false;

	int32_t countDownTime_ = 240;

	int displayStartTime_ = 0;
	int displayFinishTime_ = 0;



	Audio* finishSE_ = nullptr;
	uint32_t finishSEHandle_ = 0u;
	int32_t finishTime_ = 0;

};



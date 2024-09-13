#include "GameScene.h"

#include <imgui.h>
#include "ResultScene/ResultScene.h"

#include "GameManager.h"
#include "ModelManager.h"
#include <TextureManager.h>
#include <numbers>
#include <VectorCalculation.h>
#include "../External/TsumiInput/TInput.h"
#include <Record/Record.h>
#include "GameAudioScene/GameAudioScene.h"
#include "GameScene/GameAudioScene/GameAudioSceneFinish.h"
//コンストラクタ
GameScene::GameScene()
{
#pragma region System

	// モデルマネージャー
	modelManager_ = ModelManager::GetInstance();

	// 入力
	input_ = Input::GetInstance();
	tInput_ = TInput::GetInstance();

	//記録
	record_ = Record::GetInstance();

#pragma endregion 
}


void GameScene::Initialize() {

	//あらかじめ読み込んでおく
	PreLoad();


	/* ----- FollowCamera フォローカメラ ----- */
	uint32_t followCameraModelHD = modelManager_->LoadModelFile("Resources/FollowCamera", "FollowCamera.obj");
	followCamera_ = std::make_unique<FollowCamera>(followCameraModelHD);
	followCamera_->Init();
	//カメラ
	camera_ = followCamera_->GetCameraData();

	/* ----- Player プレイヤー ----- */
	PlayerAssetsHandle handles = {
		.head = modelManager_->LoadModelFile("Resources/Game/Player/Head","Head.obj"),
		.torso = modelManager_->LoadModelFile("Resources/Game/Player/Drill","Drill.obj"),
		.crack = modelManager_->LoadModelFile("Resources/Player/Effects/CrackEffect", "CrackEffect.obj"),
	};
	player_ = std::make_unique<Player>(handles);
	player_->Init();
	// PlayerにFollowCameraを渡す
	player_->SetFollowCamera(followCamera_.get());
	// FollowCameraにPlayerを渡す
	followCamera_->SetPlayer(player_.get());


	//平行光源
	directtionalLight_.Initialize();
	directtionalLight_.direction_ = { .x = 0.0f,.y = -1.0f,.z = 0.0f };

	



	//地面
	uint32_t groundModelHandle = modelManager_->LoadModelFile("Resources/Game/Stage","Ground.obj");
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModelHandle);


	//天球
	uint32_t skydomeModelHandle = modelManager_->LoadModelFile("Resources/Game/Stage","Stage.obj");
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModelHandle);


	// Groundの四隅の座標をPlayerに渡す
	player_->SetGroundCorners(
		ground_->GetLeftBack(), ground_->GetRightBack(),
		ground_->GetLeftFront(), ground_->GetRightFront());

	//敵
	uint32_t rockEnemyModelhandle = modelManager_->LoadModelFile("Resources/Game/Enemy/RockEnemy", "Rock.obj");
	uint32_t feEnemyModelhandle = modelManager_->LoadModelFile("Resources/Game/Enemy/FeEnemy", "Fe.obj");

	//ステージの座標を取得
	Vector3 stageLeftBack = ground_->GetLeftBack();
	Vector3 stageRightBack = ground_->GetRightBack();
	Vector3 stageLeftFront = ground_->GetLeftFront();
	Vector3 stageRightFront = ground_->GetRightFront();


	//敵管理クラス
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetStageRectPosition(stageLeftBack, stageRightBack, stageLeftFront, stageRightFront);

	enemyManager_->Initialize(rockEnemyModelhandle, feEnemyModelhandle);


	gameUI_ = std::make_unique<GameUI>();
	gameUI_->Initialize();


	//衝突判定管理クラスの初期化
	collisionManager_ = std::make_unique<CollisionManager>();


	//平行光源
	directtionalLight_.Initialize();
	///ポストエフェクト
	back_ = std::make_unique<BackText>();
	back_->Initialize();



	//カウントダウン
	uint32_t count[3] = {};
	count[2] = TextureManager::GetInstance()->LoadTexture("Resources/Number/3.png");
	count[1] = TextureManager::GetInstance()->LoadTexture("Resources/Number/2.png");
	count[0] = TextureManager::GetInstance()->LoadTexture("Resources/Number/1.png");
	for (int i = 0; i < 3; ++i) {
		countDown_[i].reset(Sprite::Create(count[i], {600,300,0.0f}));
		countDown_[i]->SetInvisible(true);
	}
	
	countDownTime_ = 240;

	finishSE_ = Audio::GetInstance();
	finishSEHandle_ = finishSE_->LoadWave("Resources/Audio/Game/Finish.wav");
	


	countSE_ = Audio::GetInstance();
	countSEHandle_ = countSE_->LoadWave("Resources/Audio/Game/Count.wav");

	startSE_ = Audio::GetInstance();
	startSEHandle_ = startSE_->LoadWave("Resources/Audio/Game/Start.wav");





	gameAudioManager_ = std::make_unique<GameAudioManager>();
	gameAudioManager_->Initialize();

	uint32_t whiteHandle= TextureManager::GetInstance()->LoadTexture("Resources/Back/White.png");
	white_.reset(Sprite::Create(whiteHandle, { 0.0f,0.0f,0.0f }));

	uint32_t startHandle = TextureManager::GetInstance()->LoadTexture("Resources/Game/Start.png");
	startSprite_.reset(Sprite::Create(startHandle, { 0.0f,0.0f,0.0f }));
	startSprite_->SetInvisible(true);

	uint32_t endhandle = TextureManager::GetInstance()->LoadTexture("Resources/Game/End.png");

	endSprite_.reset(Sprite::Create(endhandle, { 0.0f,0.0f,0.0f }));

	condition_ = FadeIn;
}


void GameScene::Update(GameManager* gameManager) {

	
	std::list<Enemy*> enemyes = enemyManager_->GetEnemyList();

	white_->SetTransparency(whiteAlpha_);



	/* ----- FollowCamera フォローカメラ ----- */
	followCamera_->Update();
	camera_ = followCamera_->GetCameraData();

	//スコア
	int32_t score = record_->GetTotalScore();



	record_->Update();

#pragma region UI
	//スコアの設定
	gameUI_->SetScore(score);
	//更新
	gameUI_->Update();
#pragma endregion

	//地面の更新
	ground_->Update();

	skydome_->Update();

	//ライトの更新




	directtionalLight_.Update();


	if (isFadeIn_ == true) {
		whiteAlpha_ -= 0.01f;
		if (whiteAlpha_ < 0.0f) {
			whiteAlpha_ = 0.0f;


			countSE_ = Audio::GetInstance();
			countSEHandle_ = countSE_->LoadWave("Resources/Audio/Game/Count.wav");

			startSE_ = Audio::GetInstance();
			startSEHandle_ = startSE_->LoadWave("Resources/Audio/Game/Start.wav");


			countDownTime_ -=1;

			if (countDownTime_ <= 240 && countDownTime_ > 180) {
				if (countDownTime_ == 239) {
					countSE_->PlayWave(countSEHandle_, false);
					countSE_->ChangeVolume(countSEHandle_, 0.6f);

				}
				countDown_[2]->SetInvisible(false);
			}
			else if (countDownTime_ <= 180 && countDownTime_ > 120) {
				if (countDownTime_ == 180) {
					countSE_->PlayWave(countSEHandle_, false);
					countSE_->ChangeVolume(countSEHandle_, 0.6f);

				}
				countDown_[2]->SetInvisible(true);
				countDown_[1]->SetInvisible(false);
			}
			else if (countDownTime_ <= 120 && countDownTime_ > 60) {
				if (countDownTime_ ==120) {
					countSE_->PlayWave(countSEHandle_, false);
					countSE_->ChangeVolume(countSEHandle_, 0.6f);

				}

				countDown_[1]->SetInvisible(true);
				countDown_[0]->SetInvisible(false);
			}
			else if (countDownTime_ <= 60 && countDownTime_ > 0) {
				if (countDownTime_ == 60) {
					startSE_->PlayWave(startSEHandle_, false);
					startSE_->ChangeVolume(startSEHandle_, 0.6f);

				}
				
				countDown_[0]->SetInvisible(true);
				startSprite_->SetInvisible(false);
			}



			
			if (countDownTime_ < 0) {
				startSprite_->SetInvisible(true);
				isGamePlay_ = true;
			}

			
		}
	}

	
	
	


	if (isGamePlay_ == true) {
		gameChange_ += 1;
		if (gameChange_==1) {
			gameAudioManager_->ChangeGameAudioScene(new GameAudioScene());

		}
		
		isFadeIn_ = false;
		gameUI_->SetIsTimeStart(true);

		//衝突管理クラスのクリア
		collisionManager_->ClearList();



		//プレイヤーの攻撃
		if (player_->IsStomping() == true) {
			collisionManager_->RegisterList(player_->GetPlayerAttack());
		}
		//本体
		collisionManager_->RegisterList(player_.get());

#pragma region 敵
		//リストの取得

		for (Enemy* enemy : enemyes) {
			//本体
			collisionManager_->RegisterList(enemy);

			//攻撃
			if (enemy->GetIsAttack() == true) {
				collisionManager_->RegisterList(enemy->GetEnemyAttackCollision());
			}

		}



		//敵管理クラスの更新
		Vector3 playerPosition = player_->GetWorldPosition();
		enemyManager_->SetPlayerPosition(playerPosition);

		enemyManager_->Update();
		enemyManager_->DeleteEnemy();


#pragma endregion


		//衝突チェック
		collisionManager_->CheckAllCollision();

	}





	gameAudioManager_->Update();


	/* ----- Player プレイヤー ----- */
	player_->Update();


	/* ----- FollowCamera フォローカメラ ----- */
	followCamera_->Update();
	camera_ = followCamera_->GetCameraData();


	
	/* ----- Input 入力関連処理 ----- */
	FuncInput();



	//制限時間が過ぎたらResultへ
	if (gameUI_->GetIsTimeOver() == true) {
		isGamePlay_ = false;
		isFinishGame_ = true;
	}

	if (isFinishGame_ == true) {

		finishTime_ += 1;
		if (finishTime_ == 1) {
			finishSE_->PlayWave(finishSEHandle_, false);
			finishSE_->ChangeVolume(finishSEHandle_, 0.7f);
		}

		///表示
		displayFinishTime_ += 1;
		if (displayFinishTime_ > 180) {
			isFadeOut = true;
		}




	}


	if (isFadeOut == true) {

		gameAudioManager_->ChangeGameAudioScene(new GameAudioSceneFinish());

		whiteAlpha_ += 0.01f;
		if (whiteAlpha_ > 1.0f) {
			whiteAlpha_ = 1.0f;
			gameManager->ChangeScene(new ResultScene());
			return;
		}
	}



	
	
}

void GameScene::DrawSpriteBack() {

}

void GameScene::DrawObject3D() {

	skydome_->Draw(camera_, directtionalLight_);

	//地面の描画
	ground_->Draw(camera_, directtionalLight_);

	//敵の描画
	enemyManager_->Draw(camera_, directtionalLight_);

	//----- Player プレイヤー ----- //
	player_->Draw3D(camera_, directtionalLight_);
}

void GameScene::PreDrawPostEffectFirst() {
	back_->PreDraw();
}

void GameScene::DrawPostEffect() {
	back_->Draw();
}

void GameScene::DrawSprite() {


	startSprite_->Draw();
	if (isFinishGame_ == true && displayFinishTime_ <= 180) {
		endSprite_->Draw();
	}


	for (int i = 0; i < 3; ++i) {
		countDown_[i]->Draw();
	}

	gameUI_->Draw();
	white_->Draw();
}

GameScene::~GameScene() {

}



/// <summary>
/// 入力関連処理
/// </summary>
void GameScene::FuncInput()
{
	// stickの入力を取得 
	input_->GetJoystickState(joyState_);

	// カメラの操作
	followCamera_->FuncStickFunc(joyState_);

	// 入力があれば移動
	player_->FuncStickFunc(joyState_);

	// Aボタンの入力
	if (tInput_->Trigger(PadData::B)) {

		
		

		// Aボタンが押された時の処理
		player_->FuncAButton();
	}
}

void GameScene::PreLoad(){
	ModelManager::GetInstance()->LoadModelFile("Resources/Game/Enemy/FeEnemy", "FeBreak.obj");
	ModelManager::GetInstance()->LoadModelFile("Resources/Game/Enemy/RockEnemy", "RockBreak.obj");
}

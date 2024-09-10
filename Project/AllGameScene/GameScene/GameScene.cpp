#include "GameScene.h"

#include <imgui.h>
#include "ResultScene/ResultScene.h"

#include "GameManager.h"
#include "ModelManager.h"
#include <TextureManager.h>
#include <numbers>
#include <VectorCalculation.h>
#include "../External/TsumiInput/TInput.h"


//コンストラクタ
GameScene::GameScene()
{
#pragma region System

	// モデルマネージャー
	modelManager_ = ModelManager::GetInstance();

	// 入力
	input_ = Input::GetInstance();
	tInput_ = TInput::GetInstance();

#pragma endregion 
}


void GameScene::Initialize() {

	/* ----- FollowCamera フォローカメラ ----- */
	uint32_t followCameraModelHD = modelManager_->LoadModelFile("Resources/FollowCamera", "FollowCamera.obj");
	followCamera_ = std::make_unique<FollowCamera>(followCameraModelHD);
	followCamera_->Init();
	//カメラ
	camera_ = followCamera_->GetCameraData();

	/* ----- Player プレイヤー ----- */
	PlayerAssetsHandle handles = {
		.player = modelManager_->LoadModelFile("Resources/Player", "Player.obj"),
		.stompSpeed = modelManager_->LoadModelFile("Resources/Player/Effects/StompSpeedEffect", "StompSpeedEffect.obj"),
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
	uint32_t groundModelHandle = modelManager_->LoadModelFile("Resources/Sample/Ground", "Ground.obj");
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModelHandle);

	// Groundの四隅の座標をPlayerに渡す
	player_->SetGroundCorners(
		ground_->GetLeftBack(), ground_->GetRightBack(), 
		ground_->GetLeftFront(), ground_->GetRightFront());

	//敵
	uint32_t rockEnemyModelhandle = modelManager_->LoadModelFile("Resources/Game/Enemy/RockEnemy","Rock.obj");
	uint32_t feEnemyModelhandle = modelManager_->LoadModelFile("Resources/Game/Enemy/FeEnemy","Fe.obj");

	//ステージの座標を取得
	Vector3 stageLeftBack = ground_->GetLeftBack();
	Vector3 stageRightBack = ground_->GetRightBack();
	Vector3 stageLeftFront = ground_->GetLeftFront();
	Vector3 stageRightFront = ground_->GetRightFront();


	//敵管理クラス
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetStageRectPosition(stageLeftBack, stageRightBack, stageLeftFront, stageRightFront);

	enemyManager_->Initialize(rockEnemyModelhandle, feEnemyModelhandle);
	




	//衝突判定管理クラスの初期化
	collisionManager_ = std::make_unique<CollisionManager>();
	

	//平行光源
	directtionalLight_.Initialize();
	directtionalLight_.direction_ = { .x = 0.0f,.y = -1.0f,.z = 0.0f };

	///ポストエフェクト
	back_ = std::make_unique<BackText>();
	back_->Initialize();

}


void GameScene::Update(GameManager* gameManager) {

	//衝突管理クラスのクリア
	collisionManager_->ClearList();


#ifdef _DEBUG



	ImGui::Begin("ゲーム");
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, 3.0f, -3.0f);
	ImGui::SliderFloat3("Position", &camera_.translate_.x, -10.0f, 10.0f);
	ImGui::End();
#endif // _DEBUG

	//仮置き
	//スペースキーで次のシーンへ
	if (Input::GetInstance()->IsTriggerKey(DIK_L) == true) {
		gameManager->ChangeScene(new ResultScene());
		return;
	}

	/* ----- FollowCamera フォローカメラ ----- */
	followCamera_->Update();
	camera_ = followCamera_->GetCameraData();
	
	/* ----- Player プレイヤー ----- */
	player_->Update();

	/* ----- Input 入力関連処理 ----- */
	FuncInput();


	#pragma region 敵
	//リストの取得
	std::list<Enemy*> enemyes = enemyManager_->GetEnemyList();
	for (Enemy* enemy : enemyes) {
		//本体
		collisionManager_->RegisterList(enemy);

		//攻撃
		collisionManager_->RegisterList(enemy->GetEnemyAttackCollision());
	}
	


	//敵管理クラスの更新
	Vector3 playerPosition = player_->GetWorldPos();
	enemyManager_->SetPlayerPosition(playerPosition);

	enemyManager_->Update();
	enemyManager_->DeleteEnemy();
	

#pragma endregion

	//衝突チェック
	collisionManager_->CheckAllCollision();


	//地面の更新
	ground_->Update();

	//ライトの更新
	directtionalLight_.Update();
}

void GameScene::DrawSpriteBack(){

}

void GameScene::DrawObject3D(){
	//skydome_->Draw(camera_);

	/* ----- FollowCamera フォローカメラ ----- */
	followCamera_->Draw3D(camera_, directtionalLight_);

	/* ----- Player プレイヤー ----- */
	player_->Draw3D(camera_, directtionalLight_);
	
	//地面の描画
	ground_->Draw(camera_, directtionalLight_);
	
	//敵の描画
	enemyManager_->Draw(camera_, directtionalLight_);


}

void GameScene::PreDrawPostEffectFirst(){
	back_->PreDraw();
}

void GameScene::DrawPostEffect(){
	back_->Draw();
}

void GameScene::DrawSprite() {


}

GameScene::~GameScene(){

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
	if (tInput_->Trigger(PadData::A)) {

		// Aボタンが押された時の処理
		player_->FuncAButton();
	}
}

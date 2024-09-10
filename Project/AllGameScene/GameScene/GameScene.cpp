#include "GameScene.h"

#include <imgui.h>
#include <Input.h>
#include "LoseScene/LoseScene.h"

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

	/* ----- Player プレイヤー ----- */
	uint32_t playerModelHD = modelManager_->LoadModelFile("Resources/Player", "Player.obj");
	player_ = std::make_unique<Player>(playerModelHD);
	player_->Init();


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

	Vector3 stageLeftBack = ground_->GetLeftBack();
	Vector3 stageRightBack = ground_->GetRightBack();
	Vector3 stageLeftFront = ground_->GetLeftFront();
	Vector3 stageRightFront = ground_->GetRightFront();


	//敵管理クラス
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetStageRectPosition(stageLeftBack, stageRightBack, stageLeftFront, stageRightFront);

	enemyManager_->Initialize(rockEnemyModelhandle, feEnemyModelhandle);
	





	collisionManager_ = std::make_unique<CollisionManager>();
	

	//平行光源
	directtionalLight_.Initialize();
	directtionalLight_.direction_ = { .x = 0.0f,.y = -1.0f,.z = 0.0f };


	//カメラ
	camera_.Initialize();
	camera_.rotate_ = { .x = 0.24f,.y = 0.0f,.z = 0.0f };
	camera_.translate_ = { .x = 0.0f,.y = 0.2f,.z = 0.0f };

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
		gameManager->ChangeScene(new LoseScene());
		return;
	}

	
	/* ----- Player プレイヤー ----- */
	player_->Update();
	PlayerInput();

	//リストの取得
	std::list<Enemy*> enemyes = enemyManager_->GetEnemyList();
	for (Enemy* enemy : enemyes) {
		//本体
		collisionManager_->RegisterList(enemy);

		//攻撃
		collisionManager_->RegisterList(enemy->GetEnemyAttackCollision());
	}
	


	//プレイヤーに追従する
	Vector3 cameraOffset = VectorCalculation::Add(
		{ player_->GetWorldPos().x, 0.0f, player_->GetWorldPos().z },
		{ 0.0, 20.0f, -60.0f });
	camera_.translate_ = cameraOffset;


	//敵管理クラスの更新
	Vector3 playerPosition = player_->GetWorldPos();
	enemyManager_->SetPlayerPosition(playerPosition);

	enemyManager_->Update();
	enemyManager_->DeleteEnemy();
	

	//衝突チェック
	collisionManager_->CheckAllCollision();


	//地面の更新
	ground_->Update();

	//ライトの更新
	directtionalLight_.Update();

	//カメラの更新
	camera_.Update();

	

}

void GameScene::DrawSpriteBack(){

}

void GameScene::DrawObject3D(){
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
/// プレイヤーの移動処理
/// </summary>
void GameScene::PlayerInput()
{
	// 移動処理
	if (input_->GetJoystickState(joyState_)) {

		// 入力があれば移動
		//player_->Move(joyState_);
		player_->FuncStickFunc(joyState_);
	}

	// ジャンプ処理
	if (tInput_->Trigger(PadData::A)) {

		// Aボタンが押された時の処理
		player_->FuncAButton();
	}
}

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
	playe_ = std::make_unique<Player>(playerModelHD);
	playe_->Init();


	//平行光源
	directtionalLight_.Initialize();
	directtionalLight_.direction_ = { .x = 0.0f,.y = -1.0f,.z = 0.0f };
	



	//下書きプレイヤー
	uint32_t draftPlayer = ModelManager::GetInstance()->LoadModelFile("Resources/Player/Model", "Player.obj");
	draftPlayer_ = std::make_unique<DraftPlayer>();
	draftPlayer_->Initialize(draftPlayer);

	playerDirection_ = {};




	//敵
	uint32_t enemyModelhandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Enemy","enemy.obj");
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModelhandle);









	//地面
	uint32_t groundModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Ground", "Ground.obj");
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModelHandle);


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

	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		draftPlayer_->SetIsJump(true);
	}


	playerDirection_ = {};

	if (Input::GetInstance()->IsPushKey(DIK_W) == true) {
		playerDirection_.z = 1.0f;
	}
	if (Input::GetInstance()->IsPushKey(DIK_S) == true) {
		playerDirection_.z = -1.0f;
	}
	if (Input::GetInstance()->IsPushKey(DIK_D) == true) {
		playerDirection_.x = 1.0f;
	}
	if (Input::GetInstance()->IsPushKey(DIK_A) == true) {
		playerDirection_.x = -1.0f;
	}


	//プレイヤーの向いている方向を設定
	draftPlayer_->SetPlayerDirection(playerDirection_);

	/* ----- Player プレイヤー ----- */
	playe_->Update();
	PlayerInput();


	directtionalLight_.Update();




	//プレイヤー(下書き)の更新
	draftPlayer_->Update();



	//プレイヤーに追従する
	Vector3 cameraOffset = VectorCalculation::Add(
		{ playe_->GetWorldPos().x, 0.0f, playe_->GetWorldPos().z },
		{ 0.0, 20.0f, -60.0f });
	camera_.translate_ = cameraOffset;





	


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
	//skydome_->Draw(camera_);

	/* ----- Player プレイヤー ----- */
	playe_->Draw3D(camera_, directtionalLight_);

	//地面の描画
	ground_->Draw(camera_, directtionalLight_);

	//下書きプレイヤーの描画
	draftPlayer_->Draw(camera_, directtionalLight_);
}

void GameScene::PreDrawPostEffectFirst(){
	back_->PreDraw();
}

void GameScene::DrawPostEffect(){
	back_->Draw();
}

void GameScene::DrawSprite() {


}

GameScene::~GameScene()
{
}


/// <summary>
/// プレイヤーの移動処理
/// </summary>
void GameScene::PlayerInput()
{
	// 移動処理
	if (input_->GetJoystickState(joyState_)) {

		// 入力があれば移動
		playe_->Move(joyState_);
	}

	// ジャンプ処理
	if (tInput_->Trigger(PadData::A)) {

		// ジャンプの処理
		playe_->JumpFunc();
	}
}

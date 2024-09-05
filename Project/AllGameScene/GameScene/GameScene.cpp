#include "GameScene.h"

#include <imgui.h>
#include <Input.h>
#include "LoseScene/LoseScene.h"

#include "GameManager.h"
#include "ModelManager.h"
#include <TextureManager.h>



//コンストラクタ
GameScene::GameScene()
{
#pragma region System

	// モデルマネージャー
	modelManager_ = ModelManager::GetInstance();

#pragma endregion 
}


void GameScene::Initialize() {

	uint32_t groundModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Sample/Ground", "Ground.obj");
	groundModelHandle;
	//skydome_ = std::make_unique<Skydome>();
	//skydome_->Initialize(groundModelHandle);


	/* ----- Player プレイヤー ----- */
	uint32_t playerModelHD = modelManager_->LoadModelFile("Resources/Player", "Player.obj");
	playe_ = std::make_unique<Player>(playerModelHD);
	playe_->Init();


	//平行光源
	directtionalLight_.Initialize();
	directtionalLight_.direction_ = { .x = 0.0f,.y = -1.0f,.z = 0.0f };
	
	camera_.Initialize();


	back_ = std::make_unique< BackText>();
	back_->Initialize();

}

void GameScene::Update(GameManager* gameManager) {


#ifdef _DEBUG
	ImGui::Begin("ゲーム");
	ImGui::SliderFloat3("Position", &camera_.translate_.x, -30.0f, 30.0f);
	ImGui::End();
#endif // _DEBUG

	//仮置き
	//スペースキーで次のシーンへ
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		gameManager->ChangeScene(new LoseScene());
		return;
	}


	/* ----- Player プレイヤー ----- */
	playe_->Update();


	directtionalLight_.Update();


	camera_.Update();


	//skydome_->Update();
	//light_.Update();



}

void GameScene::DrawSpriteBack(){

}

void GameScene::DrawObject3D(){
	//skydome_->Draw(camera_);

	/* ----- Player プレイヤー ----- */
	playe_->Draw3D(camera_, directtionalLight_);

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
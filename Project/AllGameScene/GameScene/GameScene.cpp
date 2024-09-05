#include "GameScene.h"

#include <imgui.h>
#include <Input.h>
#include "LoseScene/LoseScene.h"

#include "GameManager.h"
#include "ModelManager.h"
#include <TextureManager.h>


void GameScene::Initialize() {

	



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

	const float CAMERA_MOVE_INTERVAL = 0.05f;
	if (Input::GetInstance()->IsPushKey(DIK_W) == true) {
		camera_.translate_.y += CAMERA_MOVE_INTERVAL;
	}
	if (Input::GetInstance()->IsPushKey(DIK_S) == true) {
		camera_.translate_.y -= CAMERA_MOVE_INTERVAL;
	}
	if (Input::GetInstance()->IsPushKey(DIK_RIGHT) == true) {
		camera_.translate_.x += CAMERA_MOVE_INTERVAL;
	}
	if (Input::GetInstance()->IsPushKey(DIK_LEFT) == true) {
		camera_.translate_.x -= CAMERA_MOVE_INTERVAL;
	}
	if (Input::GetInstance()->IsPushKey(DIK_UP) == true) {
		camera_.translate_.z += CAMERA_MOVE_INTERVAL;
	}
	if (Input::GetInstance()->IsPushKey(DIK_DOWN) == true) {
		camera_.translate_.z -= CAMERA_MOVE_INTERVAL;
	}


	ImGui::Begin("ゲーム");
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, 3.0f, -3.0f);
	ImGui::SliderFloat3("Position", &camera_.translate_.x, -10.0f, 10.0f);
	ImGui::End();
#endif // _DEBUG

	//仮置き
	//スペースキーで次のシーンへ
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		gameManager->ChangeScene(new LoseScene());
		return;
	}








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
	ground_->Draw(camera_, directtionalLight_);
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
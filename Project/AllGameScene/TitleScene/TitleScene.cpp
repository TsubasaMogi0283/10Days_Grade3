#include "TitleScene.h"
#include <imgui.h>
#include <Input.h>
#include "GameScene/GameScene.h"

#include "GameManager.h"
#include "ModelManager.h"
#include <TextureManager.h>


void TitleScene::Initialize(){
	modelManager_ = ModelManager::GetInstance();
	modelManager2_ = ModelManager::GetInstance();
	directtionalLight_.Initialize();
	//カメラ
	camera_.Initialize();
	camera_.translate_ = { 0.0f,0.0f,-9.8f };
	
	///ポストエフェクト
	back_ = std::make_unique<BackText>();
	back_->Initialize();
	//タイトル
	modelHandle_ =modelManager_->LoadModelFile("Resources/Title", "Title.obj");
	titleModel_.reset(Model::Create(modelHandle_));
	mtl_.Initialize();
	transform_.Initialize();
	//タイトル2
	modelHandle2_ = modelManager2_->LoadModelFile("Resources/Title", "Title2.obj");
	titleModel2_.reset(Model::Create(modelHandle2_));
	mtl2_.Initialize();
	transform2_.Initialize();
}

void TitleScene::Update(GameManager* gameManager){
	transform_.Update();
	transform2_.Update();
	directtionalLight_.Update();
	camera_.Update();
	mtl_.Update();
	mtl2_.Update();
	XINPUT_STATE joyState{};
	joyState;
	////コントローラーのBを押すと高速点滅
	//if (Input::GetInstance()->GetJoystickState(joyState) == true) {
	//
	//	//Bボタンを押したとき
	//	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
	//		bTriggerTime_ += 1;
	//
	//	}
	//	if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
	//		bTriggerTime_ = 0;
	//	}
	//
	//	if (bTriggerTime_ == 1) {
	//
	//		isFastFlash_ = true;
	//	}
	//
	//}


#ifdef _DEBUG
	ImGui::Begin("タイトル"); 
	ImGui::End();
#endif // _DEBUG



	//仮置き
	//スペースキーで次のシーンへ
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE)==true) {
		gameManager->ChangeScene(new GameScene());
		return;
	}
}

void TitleScene::DrawSpriteBack()
{
}

void TitleScene::DrawObject3D()
{
	titleModel_->Draw(transform_, camera_, mtl_, directtionalLight_);
	titleModel2_->Draw(transform2_, camera_, mtl2_, directtionalLight_);
}

void TitleScene::PreDrawPostEffectFirst(){
	back_->PreDraw();
}

void TitleScene::DrawPostEffect(){
	back_->Draw();
}

void TitleScene::DrawSprite(){


}

TitleScene::~TitleScene()
{
}

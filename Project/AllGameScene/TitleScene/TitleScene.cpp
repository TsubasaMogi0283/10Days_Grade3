#include "TitleScene.h"
#include <imgui.h>
#include <Input.h>
#include "GameScene/GameScene.h"

#include "GameManager.h"
#include "ModelManager.h"
#include <TextureManager.h>


void TitleScene::Initialize(){


	//カメラ
	camera_.Initialize();
	camera_.translate_ = { 0.0f,0.0f,-9.8f };

	///ポストエフェクト
	back_ = std::make_unique<BackText>();
	back_->Initialize();

}

void TitleScene::Update(GameManager* gameManager){

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
}

void TitleScene::PreDrawPostEffectFirst()
{
}

void TitleScene::DrawPostEffect()
{
}

void TitleScene::DrawSprite(){


}

TitleScene::~TitleScene()
{
}

#include "ResultScene.h"
#include <TextureManager.h>
#include <Input.h>
#include "TitleScene/TitleScene.h"
#include "GameManager.h"



void ResultScene::Initialize(){
	back_ = std::make_unique<BackText>();
	back_->Initialize();
}

void ResultScene::Update(GameManager* gameManager){



#ifdef _DEBUG
	ImGui::Begin("結果");
	ImGui::End();
#endif // _DEBUG

	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		gameManager->ChangeScene(new TitleScene());
		return;
	}

	

}

void ResultScene::DrawSpriteBack()
{
}

void ResultScene::DrawObject3D()
{
}

void ResultScene::PreDrawPostEffectFirst(){
	back_->PreDraw();
}

void ResultScene::DrawPostEffect(){
	back_->Draw();
}

void ResultScene::DrawSprite(){

}

ResultScene::~ResultScene()
{
}

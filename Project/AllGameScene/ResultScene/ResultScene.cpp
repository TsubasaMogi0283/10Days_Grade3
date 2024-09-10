#include "ResultScene.h"
#include <TextureManager.h>
#include <Input.h>
#include "TitleScene/TitleScene.h"
#include "GameManager.h"



void ResultScene::Initialize(){

}

void ResultScene::Update(GameManager* gameManager){



#ifdef _DEBUG
	ImGui::Begin("負け");
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

void ResultScene::PreDrawPostEffectFirst()
{
}

void ResultScene::DrawPostEffect()
{
}

void ResultScene::DrawSprite(){

}

ResultScene::~ResultScene()
{
}

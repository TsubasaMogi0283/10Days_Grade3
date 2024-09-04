#include "LoseScene.h"
#include <TextureManager.h>
#include <Input.h>
#include "TitleScene/TitleScene.h"
//#include "SampleScene/SampleScene.h"
#include "GameManager.h"



void LoseScene::Initialize(){

}

void LoseScene::Update(GameManager* gameManager){



#ifdef _DEBUG
	ImGui::Begin("負け");
	ImGui::End();
#endif // _DEBUG

	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		gameManager->ChangeScene(new TitleScene());
		return;
	}

	

}

void LoseScene::DrawSpriteBack()
{
}

void LoseScene::DrawObject3D()
{
}

void LoseScene::PreDrawPostEffectFirst()
{
}

void LoseScene::DrawPostEffect()
{
}

void LoseScene::DrawSprite(){

}

LoseScene::~LoseScene()
{
}

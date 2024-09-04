#include "GameScene.h"

#include <imgui.h>
#include <Input.h>
#include "LoseScene/LoseScene.h"

#include "GameManager.h"
#include "ModelManager.h"
#include <TextureManager.h>


void GameScene::Initialize() {

	//カメラ
	camera_.Initialize();
	camera_.translate_ = { 0.0f,0.0f,-9.8f };


}

void GameScene::Update(GameManager* gameManager) {




#ifdef _DEBUG
	ImGui::Begin("ゲーム");
	ImGui::End();
#endif // _DEBUG

	//仮置き
	//スペースキーで次のシーンへ
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		gameManager->ChangeScene(new LoseScene());
		return;
	}




}

void GameScene::DrawSpriteBack()
{
}

void GameScene::DrawObject3D()
{
}

void GameScene::PreDrawPostEffectFirst()
{
}

void GameScene::DrawPostEffect()
{
}

void GameScene::DrawSprite() {


}

GameScene::~GameScene()
{
}
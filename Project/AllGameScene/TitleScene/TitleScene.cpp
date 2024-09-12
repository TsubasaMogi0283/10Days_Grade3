#include "TitleScene.h"
#include <imgui.h>
#include <Input.h>
#include "GameScene/GameScene.h"
#include "ResultScene/ResultScene.h"

#include "GameManager.h"
#include "ModelManager.h"
#include <TextureManager.h>


void TitleScene::Initialize(){
	modelManager_ = ModelManager::GetInstance();
	modelManager2_ = ModelManager::GetInstance();
	directtionalLight_.Initialize();
	
	tInput_ = TInput::GetInstance();
	//カメラ
	camera_.Initialize();
	camera_.translate_ = { 0.0f,0.0f,-9.8f };
	
	///ポストエフェクト
	back_ = std::make_unique<BackText>();
	back_->Initialize();
	//タイトル
	modelManager_ = ModelManager::GetInstance();
	modelHandle_ =modelManager_->LoadModelFile("Resources/Title", "Title.obj");
	titleModel_.reset(Model::Create(modelHandle_));
	mtl_.Initialize();
	transform_.Initialize();
	//タイトル2
	modelManager2_ = ModelManager::GetInstance();
	modelHandle2_ = modelManager2_->LoadModelFile("Resources/Title", "Title2.obj");
	titleModel2_.reset(Model::Create(modelHandle2_));
	mtl2_.Initialize();
	transform2_.Initialize();
	//B
	modelManagerB_ = ModelManager::GetInstance();
	modelHandleB_ = modelManagerB_->LoadModelFile("Resources/Title", "B.obj");
	titleModelB_.reset(Model::Create(modelHandleB_));
	mtlB_.Initialize();
	transformB_.Initialize();
	transformB_.translate_ = { 0,-2,0 };
	///
	uint32_t Handle = ModelManager::GetInstance()->LoadModelFile("Resources/Game/Player/Head", "Head.obj");
	model_.reset(Model::Create(Handle));
	uint32_t drillHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Game/Player/Drill", "Drill.obj");
	drill_.reset(Model::Create(drillHandle));
	mtlD_.Initialize();
	transformD_.Initialize();
	transformD_.translate_ = { -0.8f,3.0f,0 };
	transformD_.scale_ = { 0.2f,0.2f,0.2f };
	//
	modelManagerDungeon_ = ModelManager::GetInstance();
	modelHandleDungeon_ = modelManagerDungeon_->LoadModelFile("Resources/Title", "Dungeon.obj");
	titleModelDungeon_.reset(Model::Create(modelHandleDungeon_));
	mtlDungeon_.Initialize();
	transformDungeon_.Initialize();
	transformDungeon_.translate_ = { 0,-5,13 };
	//
	isPlayScene_ = false;
	speed = 0.2f;
}

void TitleScene::Update(GameManager* gameManager){
	transform_.Update();
	transform2_.Update();
	transformB_.Update();
	transformD_.Update();
	transformDungeon_.Update();
	directtionalLight_.Update();
	camera_.Update();
	mtl_.Update();
	mtl2_.Update();
	mtlB_.Update();
	mtlD_.Update();
	mtlDungeon_.Update();
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
	ImGui::SliderFloat3("Position", &transformD_.translate_.x, -100.0f, 100.0f);
	ImGui::End();
#endif // _DEBUG


	//transform2_.translate_.y -= 0.1f;
	//仮置き
	//スペースキーで次のシーンへ
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE)==true|| tInput_->Trigger(PadData::B)) {
		
		isPlayScene_ = true;
		return;
	}
	if(isPlayScene_ == true){
		transformD_.translate_.y -= speed;
		transformD_.rotate_.y -= 0.2f;
		if (transformD_.translate_.y < 0.8) {
			speed = 0;
			transform2_.translate_.y -= 0.1f;
			if (transform2_.translate_.y < -3) {
				gameManager->ChangeScene(new GameScene());
			}
		}
	}
	
}

void TitleScene::DrawSpriteBack()
{
}

void TitleScene::DrawObject3D()
{
	titleModel_->Draw(transform_, camera_, mtl_, directtionalLight_);
	titleModel2_->Draw(transform2_, camera_, mtl2_, directtionalLight_);
	titleModelB_->Draw(transformB_, camera_, mtlB_, directtionalLight_);
	
	model_->Draw(transformD_, camera_, mtlD_, directtionalLight_);
	drill_->Draw(transformD_, camera_, mtlD_, directtionalLight_);
	
	titleModelDungeon_->Draw(transformDungeon_, camera_, mtlDungeon_, directtionalLight_);
	
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

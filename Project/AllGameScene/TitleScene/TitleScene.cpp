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



	uint32_t whiteHandle = TextureManager::GetInstance()->LoadTexture("Resources/Back/White.png");
	white_.reset(Sprite::Create(whiteHandle, {0.0f,0.0f,0.0f}));
	white_->SetTransparency(0.0f);




	bgmHandle = Audio::GetInstance()->LoadWave("Resources/Audio/Title/TitleBGM.wav");
	bgm_ = Audio::GetInstance();

	
	bgm_->PlayWave(bgmHandle, true);
	bgm_->ChangeVolume(bgmHandle, 0.4f);


	startSE_ = Audio::GetInstance();
	seHandle_ = startSE_->LoadWave("Resources/Audio/Decide.wav");
	startSETime_ = 0;

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


#ifdef _DEBUG
	ImGui::Begin("タイトル"); 
	ImGui::SliderFloat3("Position", &transformD_.translate_.x, -100.0f, 100.0f);
	ImGui::End();
#endif // _DEBUG
	
	theta_ += 0.05f;
	transformB_.translate_.y = std::sinf(theta_)*(0.3f)-1.5f;

	//transform2_.translate_.y -= 0.1f;
	//仮置き
	//スペースキーで次のシーンへ
	if ((Input::GetInstance()->IsTriggerKey(DIK_SPACE)==true|| tInput_->Trigger(PadData::B))&& isPlayScene_==false) {
		

		
		startSETime_ += 1;
		isPlayScene_ = true;
	}

	if (startSETime_ == 1) {
		startSETime_ = 0;
		startSE_->ChangeVolume(seHandle_,0.5f);
		startSE_->PlayWave(seHandle_, false);
	}



	if(isPlayScene_ == true){
		

		transformD_.translate_.y -= speed;
		transformD_.rotate_.y -= 0.2f;
		if (transformD_.translate_.y < 0.8) {
			speed = 0;
			transform2_.translate_.y -= 0.1f;
			
			
			

			if (transform2_.translate_.y < -3.0f) {
				
				isFadeOut_ = true;
				
			}
		}
	}


	if (isFadeOut_ == true) {
		filter_ -= 0.01f;
		whiteAlpha_ += 0.01f;

	}
	
#ifdef _DEBUG
	ImGui::Begin("SS");
	ImGui::InputFloat("A",&whiteAlpha_);
	ImGui::End();
#endif // _DEBUG


	bgm_->SetLowPassFilter(bgmHandle, filter_);
	white_->SetTransparency(whiteAlpha_);
	if (whiteAlpha_ > 1.0f) {
		bgm_->StopWave(bgmHandle);
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
	white_->Draw();

}

TitleScene::~TitleScene()
{
}

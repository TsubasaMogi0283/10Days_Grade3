#include "GameUI.h"
#include <TextureManager.h>

void GameUI::Initialize(){

	
	
	for (uint32_t i = 0; i < 10; ++i) {
		//数を文字列に変換した方が賢いよね！
		//すっきり！
		const std::string number = std::to_string(i);
		const std::string filePath = "Resources/Number/" + number + ".png";
		numberQuantity[i] = TextureManager::GetInstance()->LoadTexture(filePath);
	}

	//初期位置
	const Vector2 SCORE_INITIAL_LEFT_POSITION = {.x= 270.0f,.y=630.0f };
	//生成
	for (uint32_t i = 0; i < SCORE_DIGIT_; ++i) {
		scoreSprites_[i].reset(Sprite::Create(numberQuantity[0], {.x= i*-64.0f+SCORE_INITIAL_LEFT_POSITION.x, .y= SCORE_INITIAL_LEFT_POSITION .y,.z=0.0f}));
	}

	uint32_t hpwToHandle = TextureManager::GetInstance()->LoadTexture("Resources/Game/Howto.png");
	howTo_.reset(Sprite::Create(hpwToHandle, {0.0f,0.0f,0.0f}));

	isTimeStart_ = false;

	//時間
	time_ = (currentTime_) * SECOND;
#ifdef _DEBUG
	time_ = 3*SECOND;
#endif // _DEBUG


	//初期位置
	const Vector2 TIME_INITIAL_LEFT_POSITION = { .x = 660.0f,.y = 20.0f };
	//生成
	for (uint32_t i = 0; i < TIME_DIGIT_; ++i) {
		timeSprites_[i].reset(Sprite::Create(numberQuantity[0], { .x = i * -64.0f + TIME_INITIAL_LEFT_POSITION.x, .y = TIME_INITIAL_LEFT_POSITION.y,.z = 0.0f }));
	}
}

void GameUI::Update(){




	//時間
	if (isTimeStart_ == true) {
		time_ -= 1;

	}
	

	if (time_ < 0) {
		time_ = 0;
		isTimeOver_ = true;
	}


	currentTime_ = time_ / SECOND;

	timePlaces_[OnePlace] = currentTime_ % 10;
	timePlaces_[TenPlace] = currentTime_ / 10;


	//一の位
	scorePlaces_[OnePlace] = score_ % 10;

	//十の位
	scorePlaces_[TenPlace] = (score_ % 100- scorePlaces_[OnePlace])/10;

	//百の位
	scorePlaces_[HundredPlace] = (score_ % 1000 - scorePlaces_[TenPlace])/100;

	//千の位
	scorePlaces_[ThousandPlace] = (score_ % 10000 - scorePlaces_[HundredPlace])/1000;

	//万の位
	scorePlaces_[TenThousandPlace] = score_ / 10000;


#ifdef _DEBUG
	ImGui::Begin("ゲームUI");
	ImGui::InputInt("値", &score_);

	if (ImGui::TreeNode("位")) {
		ImGui::InputInt("一の位", &scorePlaces_[OnePlace]);
		ImGui::InputInt("十の位", &scorePlaces_[TenPlace]);
		ImGui::InputInt("百の位", &scorePlaces_[HundredPlace]);
		ImGui::InputInt("千の位", &scorePlaces_[ThousandPlace]);
		ImGui::InputInt("万の位", &scorePlaces_[TenThousandPlace]);

		
		ImGui::TreePop();
	}

	ImGui::InputInt("値", &time_);
	if (ImGui::TreeNode("時間")) {
		ImGui::InputInt("値", &currentTime_);
		ImGui::InputInt("一の位", &scorePlaces_[OnePlace]);
		ImGui::InputInt("十の位", &scorePlaces_[TenPlace]);


		ImGui::TreePop();
	}




	ImGui::End();
#endif // _DEBUG
}

void GameUI::Draw(){

	//時間
	for (uint32_t i = 0; i < TIME_DIGIT_; ++i) {
		timeSprites_[i]->Draw(numberQuantity[timePlaces_[i]]);
	}


	//スコア
	for (uint32_t i = 0; i < SCORE_DIGIT_; ++i) {
		scoreSprites_[i]->Draw(numberQuantity[scorePlaces_[i]]);
	}

	howTo_->Draw();
}

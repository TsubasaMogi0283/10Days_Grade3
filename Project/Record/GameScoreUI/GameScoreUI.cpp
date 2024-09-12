#include "GameScoreUI.h"
#include <TextureManager.h>

void GameScoreUI::Initialize(){

	
	
	for (uint32_t i = 0; i < 10; ++i) {
		//数を文字列に変換した方が賢いよね！
		//すっきり！
		const std::string number = std::to_string(i);
		const std::string filePath = "Resources/Number/" + number + ".png";
		numberQuantity[i] = TextureManager::GetInstance()->LoadTexture(filePath);
	}

	//初期位置
	const Vector2 INITIAL_LEFT_POSITION = {.x= 270.0f,.y=20.0f };
	//生成
	for (uint32_t i = 0; i < DIGIT_; ++i) {
		sprites_[i].reset(Sprite::Create(numberQuantity[0], {.x= i*-64.0f+INITIAL_LEFT_POSITION.x, .y= INITIAL_LEFT_POSITION .y,.z=0.0f}));
	}

}

void GameScoreUI::Update(){


	//一の位
	places_[OnePlace] = score_ % 10;

	//十の位
	places_[TenPlace] = (score_ % 100- places_[OnePlace])/10;

	//百の位
	places_[HundredPlace] = (score_ % 1000 - places_[TenPlace])/100;

	//千の位
	places_[ThousandPlace] = (score_ % 10000 - places_[HundredPlace])/1000;

	//万の位
	places_[TenThousandPlace] = score_ / 10000;


#ifdef _DEBUG
	ImGui::Begin("スコア");
	ImGui::InputInt("値", &score_);

	if (ImGui::TreeNode("位")) {
		ImGui::InputInt("一の位", &places_[OnePlace]);
		ImGui::InputInt("十の位", &places_[TenPlace]);
		ImGui::InputInt("百の位", &places_[HundredPlace]);
		ImGui::InputInt("千の位", &places_[ThousandPlace]);
		ImGui::InputInt("万の位", &places_[TenThousandPlace]);

		
		ImGui::TreePop();
	}

	ImGui::End();
#endif // _DEBUG
}

void GameScoreUI::Draw(){
	for (uint32_t i = 0; i < DIGIT_; ++i) {
		sprites_[i]->Draw(numberQuantity[places_[i]]);
	}
}

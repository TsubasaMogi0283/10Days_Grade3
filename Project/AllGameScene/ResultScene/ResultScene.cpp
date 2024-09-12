#include "ResultScene.h"
#include <TextureManager.h>
#include <Input.h>
#include "TitleScene/TitleScene.h"
#include "GameManager.h"
#include <Record/Record.h>



void ResultScene::Initialize(){

	for (uint32_t i = 0; i < 10; ++i) {
		//数を文字列に変換した方が賢いよね！
		//すっきり！
		const std::string number = std::to_string(i);
		const std::string filePath = "Resources/Number/" + number + ".png";
		numberQuantity[i] = TextureManager::GetInstance()->LoadTexture(filePath);
	}

	//初期位置
	const Vector2 SCORE_INITIAL_LEFT_POSITION = { .x = 720.0f,.y = 200.0f };
	//生成
	for (uint32_t i = 0; i < SCORE_DIGIT_; ++i) {
		scoreSprites_[i].reset(Sprite::Create(numberQuantity[0], { .x = i * -64.0f + SCORE_INITIAL_LEFT_POSITION.x, .y = SCORE_INITIAL_LEFT_POSITION.y,.z = 0.0f }));
	}


	scoreFromRecord_ = Record::GetInstance()->GetTotalScore();


	//ポストエフェクト
	back_ = std::make_unique<BackText>();
	back_->Initialize();
}

void ResultScene::Update(GameManager* gameManager){



	switch (condition_) {
	case ScoreUp:


		//増えている時
		if (isSameScore_ == false) {
			resultScore_ += 9;

			//スキップ
			if (Input::GetInstance()->GetJoystickState(joyState_) == true) {

				//Bボタンを押したとき
				if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
					bTriggerTime_ += 1;

				}
				if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
					bTriggerTime_ = 0;
				}

				if (bTriggerTime_ == 1) {
					resultScore_ = scoreFromRecord_;
					isSameScore_ = true;
				}

			}

		}
		//大きくなったら増やすのをやめる
		if (resultScore_ > scoreFromRecord_) {
			resultScore_ = scoreFromRecord_;
			isSameScore_ = true;
			//DisaplayScoreへ
			condition_ = DisaplayScore;
		}


		break;


	case DisaplayScore:


		displayTime_ += 1;


		//次へ進む
		if (displayTime_ > 180) {
			//
			if (Input::GetInstance()->GetJoystickState(joyState_) == true) {

				//Bボタンを押したとき
				if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
					bTriggerTime_ += 1;

				}
				if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
					bTriggerTime_ = 0;
				}

				if (bTriggerTime_ == 1) {
					condition_ = DisaplayScore;
				}


			}

		}

		


		break;



	case SelectNextScene:

		//次にどのシーンへ行くかを選択する



		if (Input::GetInstance()->GetJoystickState(joyState_) == true) {

			//Bボタンを押したとき
			if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
				bTriggerTime_ += 1;

			}
			if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
				bTriggerTime_ = 0;
			}

			if (bTriggerTime_ == 1) {
				condition_ = DisaplayScore;
			}


		}

		//リプレイ
		GameManager::nextSceneFromResultScene_ = ReplayGame;
		
		//タイトルへ
		GameManager::nextSceneFromResultScene_ = ReturnToTitle;

		break;

	}

	



	

	

	//一の位
	scorePlaces_[OnePlace] = resultScore_ % 10;

	//十の位
	scorePlaces_[TenPlace] = (resultScore_ % 100 - scorePlaces_[OnePlace]) / 10;

	//百の位
	scorePlaces_[HundredPlace] = (resultScore_ % 1000 - scorePlaces_[TenPlace]) / 100;

	//千の位
	scorePlaces_[ThousandPlace] = (resultScore_ % 10000 - scorePlaces_[HundredPlace]) / 1000;

	//万の位
	scorePlaces_[TenThousandPlace] = resultScore_ / 10000;




#ifdef _DEBUG
	ImGui::Begin("ゲームUI");
	ImGui::InputInt("値", &scoreFromRecord_);

	if (ImGui::TreeNode("位")) {
		ImGui::InputInt("一の位", &scorePlaces_[OnePlace]);
		ImGui::InputInt("十の位", &scorePlaces_[TenPlace]);
		ImGui::InputInt("百の位", &scorePlaces_[HundredPlace]);
		ImGui::InputInt("千の位", &scorePlaces_[ThousandPlace]);
		ImGui::InputInt("万の位", &scorePlaces_[TenThousandPlace]);


		ImGui::TreePop();
	}

	ImGui::Checkbox("isSameScore_", &isSameScore_);



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
	//スコア
	for (uint32_t i = 0; i < SCORE_DIGIT_; ++i) {
		scoreSprites_[i]->Draw(numberQuantity[scorePlaces_[i]]);
	}
}

ResultScene::~ResultScene()
{
}

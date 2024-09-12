#include "ResultScene.h"
#include <TextureManager.h>
#include <Input.h>
#include "TitleScene/TitleScene.h"
#include "GameManager.h"
#include <Record/Record.h>
#include "GameScene/GameScene.h"
#include <SingleCalculation.h>



void ResultScene::Initialize(){

	for (uint32_t i = 0; i < 10; ++i) {
		//数を文字列に変換した方が賢いよね！
		//すっきり！
		const std::string number = std::to_string(i);
		const std::string filePath = "Resources/Number/" + number + ".png";
		numberQuantity[i] = TextureManager::GetInstance()->LoadTexture(filePath);
	}

	//初期位置
	const Vector2 SCORE_INITIAL_LEFT_POSITION = { .x = 920.0f,.y = 160.0f };
	//生成
	for (uint32_t i = 0; i < SCORE_DIGIT_; ++i) {
		scoreSprites_[i].reset(Sprite::Create(numberQuantity[0], { .x = i * -64.0f + SCORE_INITIAL_LEFT_POSITION.x, .y = SCORE_INITIAL_LEFT_POSITION.y,.z = 0.0f }));
	}

	uint32_t backGround = TextureManager::GetInstance()->LoadTexture("Resources/Result/ResultBackGround.png");
	backGround_.reset(Sprite::Create(backGround, { 0.0f,0.0f,0.0f }));

	//B次
	uint32_t bNextText = TextureManager::GetInstance()->LoadTexture("Resources/Result/BNext.png");
	bNext_.reset(Sprite::Create(bNextText, { 0.0f,0.0f,0.0f }));

	//白
	uint32_t white = TextureManager::GetInstance()->LoadTexture("Resources/Back/White.png");
	white_.reset(Sprite::Create(white, { 0.0f,0.0f,0.0f }));


	//プレイヤーキャラクター
	normalFace_ = TextureManager::GetInstance()->LoadTexture("Resources/Result/ScorePlayer.png");
	hFace_= TextureManager::GetInstance()->LoadTexture("Resources/Result/ScorePlayer2.png");
	face_ = normalFace_;
	player_.reset(Sprite::Create(face_, {0.0f,0.0f,0.0f}));


	lowestRank_ = TextureManager::GetInstance()->LoadTexture("Resources/Result/character.png");
	tonkachiRank_ = TextureManager::GetInstance()->LoadTexture("Resources/Result/character1.png");
	hummerRank_ = TextureManager::GetInstance()->LoadTexture("Resources/Result/character2.png");
	rankName_ = lowestRank_;
	rank_.reset(Sprite::Create(rankName_, { 0.0f,0.0f,0.0f }));
	


	//スコア
	scoreFromRecord_ = Record::GetInstance()->GetTotalScore();

#ifdef _DEBUG
	scoreFromRecord_ = 1500;

#endif // _DEBUG

	
	isSameScore_ = false;
	//ポストエフェクト
	back_ = std::make_unique<BackText>();
	back_->Initialize();










	roll_ = Audio::GetInstance();
	rollHandle_=roll_->LoadWave("Resources/Audio/ScoreUpSE.wav");

}

void ResultScene::Update(GameManager* gameManager){

	switch (condition_) {
	default:
	case FadeInResult:
		bNext_->SetInvisible(true);
		rank_->SetInvisible(true);
		
		whiteAlpha_ -= 0.01f;

		if (whiteAlpha_ < 0.0f) {
			whiteAlpha_ = 0.0f;
			condition_ = ScoreUp;
		}
		break;



	case ScoreUp:
		//増えている時
		if (isSameScore_ == false) {
			resultScore_ += 8;


			rollTime_ += 1;
			if (rollTime_ == 1) {
				roll_->PlayWave(rollHandle_, false);
			}

			upTime_+=1.0f/100.0f;
			if (upTime_ >= 0.0f && upTime_ <= 1.0f) {
				resultScore_=SingleCalculation::Lerp(0, scoreFromRecord_, upTime_);

			}
			
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
			roll_->StopWave(rollHandle_);
			resultScore_ = scoreFromRecord_;
			isSameScore_ = true;
			//DisaplayScoreへ
			condition_ = DisaplayScore;
		}

		break;


	case DisaplayScore:


		displayTime_ += 1;


		rank_->SetInvisible(false);

		//何やってんだお前！
		//エッチなものは禁止、焼却！！
		if (resultScore_ == 0) {
			face_ = hFace_;
		}

		if (resultScore_ > 0 && resultScore_ <= 500) {
			rankName_ = lowestRank_;
		}
		else if (resultScore_ > 500 && resultScore_ <= 900) {
			rankName_ = tonkachiRank_;
		}
		else if (resultScore_ > 900 && resultScore_ <= 1500) {
			rankName_ = hummerRank_;
		}



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
					condition_ = SelectNextScene;

#ifdef _DEBUG
					condition_ = ChangeNextSceneFromResult;
#endif // _DEBUG


				}


			}


			//点滅
			flashingTime_ += 1;
			if (flashingTime_ / 30 == 0) {
				bNext_->SetInvisible(false);
			}
			else if (flashingTime_ / 30 == 1) {
				bNext_->SetInvisible(true);
			}



			if (flashingTime_ > 60) {
				flashingTime_ = 0;
			}


		}

		


		break;



	case SelectNextScene:


		bNext_->SetInvisible(true);
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
				condition_ = ChangeNextSceneFromResult;
			}
		}
		break;



	case ChangeNextSceneFromResult:
		//リプレイ
		
		

		whiteAlpha_ += 0.01f;

		if (whiteAlpha_ > 1.0f) {
			whiteAlpha_ = 1.0f;

#ifdef _DEBUG
			gameManager->ChangeScene(new TitleScene());
			return;
#endif // _DEBUG


			////タイトルへ
			//if (Record::GetInstance()->GetNextSceneFromResult() == ReturnToTitle) {
			//	gameManager->ChangeScene(new TitleScene());
			//	return;
			//}
			////ゲームシーンへ
			//else if (Record::GetInstance()->GetNextSceneFromResult() == ReplayGame) {
			//	gameManager->ChangeScene(new GameScene());
			//	return;
			//}

		}


		break;

	}

	


	//透明
	white_->SetTransparency(whiteAlpha_);

	

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
	
	backGround_->Draw();
	//スコア
	for (uint32_t i = 0; i < SCORE_DIGIT_; ++i) {
		scoreSprites_[i]->Draw(numberQuantity[scorePlaces_[i]]);
	}

	//ランク
	rank_->Draw(rankName_);

	//Bボタン
	bNext_->Draw();

	player_->Draw(face_);

	white_->Draw();

}

ResultScene::~ResultScene()
{
}

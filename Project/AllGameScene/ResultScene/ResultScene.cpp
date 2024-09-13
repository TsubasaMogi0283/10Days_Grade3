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
	drillRank_= TextureManager::GetInstance()->LoadTexture("Resources/Result/character3.png");
	pbRank_ = TextureManager::GetInstance()->LoadTexture("Resources/Result/character4.png");
	rankName_ = lowestRank_;
	rank_.reset(Sprite::Create(rankName_, { 0.0f,0.0f,0.0f }));
	

	
	

	uint32_t speechHandle= TextureManager::GetInstance()->LoadTexture("Resources/Result/Chara/comment.png");
	//吹き出し
	speech_.reset(Sprite::Create(speechHandle, { 0.0f,0.0f,0.0f }));
	


	//コメント

	deadComment_ = TextureManager::GetInstance()->LoadTexture("Resources/Result/Chara/comment1.png");
	tonkachiComment_ = TextureManager::GetInstance()->LoadTexture("Resources/Result/Chara/comment2.png");
	hummerComment_ = TextureManager::GetInstance()->LoadTexture("Resources/Result/Chara/comment3.png");
	drillComment_= TextureManager::GetInstance()->LoadTexture("Resources/Result/Chara/comment4.png");
	pbComment_ = TextureManager::GetInstance()->LoadTexture("Resources/Result/Chara/comment5.png");

	commentHandle_ = deadComment_;
	comment_.reset(Sprite::Create(commentHandle_, { 0.0f,0.0f,0.0f }));
	comment_->SetInvisible(true);

	//スコア
	scoreFromRecord_ = Record::GetInstance()->GetTotalScore();

#ifdef _DEBUG
	scoreFromRecord_ = 3000;

#endif // _DEBUG

	
	isSameScore_ = false;
	//ポストエフェクト
	back_ = std::make_unique<BackText>();
	back_->Initialize();





	bgm_ = Audio::GetInstance();
	bgmHandle_ =bgm_->LoadWave("Resources/Audio/Result/ResultBGM.wav");
	bgm_->PlayWave(bgmHandle_, true);
	bgm_->ChangeVolume(bgmHandle_,0.6f);
	filter_ = 1.0f;




	roll_ = Audio::GetInstance();
	rollHandle_=roll_->LoadWave("Resources/Audio/ScoreUpSE.wav");

	rankVoice_ = Audio::GetInstance();
	rankVoiceHandle_[0] = rankVoice_->LoadWave("Resources/Audio/Result/Dead.wav");
	rankVoiceHandle_[1] = rankVoice_->LoadWave("Resources/Audio/Result/TonkachiRank.wav");
	rankVoiceHandle_[2] = rankVoice_->LoadWave("Resources/Audio/Result/HummerRank.wav");
	rankVoiceHandle_[3] = rankVoice_->LoadWave("Resources/Audio/Result/DrillRank.wav");
	rankVoiceHandle_[4] = rankVoice_->LoadWave("Resources/Audio/Result/PB.wav");

	decideSE_ = Audio::GetInstance();
	decideSEHandle_ = decideSE_->LoadWave("Resources/Audio/Decide.wav");
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
		comment_->SetInvisible(false);

		//何やってんだお前！
		//エッチなものは禁止、焼却！！
		//岩くず
		if (resultScore_ == 0) {
			face_ = hFace_;
			rankName_ = lowestRank_;
			commentHandle_ = deadComment_;
		}


		//トンカチ
		else if (resultScore_ > 0 && resultScore_ <= 1500) {
			rankName_ = tonkachiRank_;
			commentHandle_ = tonkachiComment_;
		}
		//ハンマー
		else if (resultScore_ > 1500 && resultScore_ <= 2500) {
			rankName_ = hummerRank_;
			commentHandle_ = hummerComment_;
		}
		//ドリル
		else if (resultScore_ > 2500 && resultScore_ <= 4000) {
			rankName_ = drillRank_;
			commentHandle_ = drillComment_;
		}
		else if (resultScore_ > 4000 ) {
			rankName_ = pbRank_;
			commentHandle_ = pbComment_;
		}


		rankTime_ += 1;
		if (rankTime_ == 1) {
			//岩くず
			if (rankName_ == lowestRank_) {
				rankVoice_->PlayWave(rankVoiceHandle_[0], false);
			}
			//とんかち
			else if (rankName_ == tonkachiRank_) {
				rankVoice_->PlayWave(rankVoiceHandle_[1], false);
			}
			//はんまー
			else if (rankName_ == hummerRank_) {
				rankVoice_->PlayWave(rankVoiceHandle_[2], false);
			}
			//どりる
			else if (rankName_ == drillRank_) {
				rankVoice_->PlayWave(rankVoiceHandle_[3], false);
			}
			//PB
			else if (rankName_ == pbRank_) {
				rankVoice_->PlayWave(rankVoiceHandle_[4], false);
			}

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

					condition_ = ChangeNextSceneFromResult;


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
		
		seTime += 1;
		if (seTime == 1) {
			decideSE_->PlayWave(decideSEHandle_, false);
			decideSE_->ChangeVolume(decideSEHandle_, 0.6f);
		}
		
		filter_ = 0.01f;



		whiteAlpha_ += 0.01f;

		if (whiteAlpha_ > 1.0f) {
			whiteAlpha_ = 1.0f;
			bgm_->StopWave(bgmHandle_);
			gameManager->ChangeScene(new TitleScene());
			return;


		}


		break;

	}

	
	bgm_->SetLowPassFilter(bgmHandle_, filter_);

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

	speech_->Draw();



	comment_->Draw(commentHandle_);
	//Bボタン
	bNext_->Draw();

	player_->Draw(face_);

	white_->Draw();

}

ResultScene::~ResultScene()
{
}

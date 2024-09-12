#pragma once
#include "IGameScene.h"

#include <memory>

#include "Sprite.h"
#include "Model.h"
#include "Camera.h"
#include <BackText.h>
#include "Input.h"

//StatePatternを使う時は必ず前方宣言をするように
class GameManager;


class ResultScene : public IGameScene {
public:

	//コンストラクタ
	ResultScene() = default;


	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

#pragma region 描画

	/// <summary>
	/// ポストエフェクト掛ける前のスプライト
	/// </summary>
	void DrawSpriteBack()override;


	/// <summary>
	/// 3Dオブジェクト
	/// </summary>
	void DrawObject3D()override;


	void PreDrawPostEffectFirst()override;
	void DrawPostEffect()override;


	/// <summary>
	/// スプライト
	/// </summary>
	void DrawSprite()override;

#pragma endregion
	/// デストラクタ
	~ResultScene();



private:
	enum NumberPlace {
		//一
		OnePlace,
		//十
		TenPlace,
		//百
		HundredPlace,
		//千
		ThousandPlace,
		//万
		TenThousandPlace,
	};





	enum ResultCondition {
		//スコアアップ
		ScoreUp,
		//表示
		DisaplayScore,
		//選択
		SelectNextScene,
		//



	};



	

private:
	Input* input_ = nullptr;
	XINPUT_STATE joyState_{};







	int32_t bTriggerTime_ = 0;



private:

	//ポストエフェクト
	//今は使わない
	std::unique_ptr<BackText> back_ = nullptr;


	//スコア
	int32_t resultScore_ = 0;
	int32_t scoreFromRecord_ = 0;
	bool isSameScore_ = false;


	//5桁が最大
	static const uint32_t SCORE_DIGIT_ = 5u;
	std::array<std::unique_ptr<Sprite>, SCORE_DIGIT_>scoreSprites_ = { nullptr };

	

	//位
	int32_t scorePlaces_[SCORE_DIGIT_] = {};
	static const uint32_t NUMBER_QUANTITY = 10;
	//スプライト
	uint32_t numberQuantity[NUMBER_QUANTITY] = {};



private:
	int32_t displayTime_ = 0;





	//状態
	int32_t condition_ = -1;
	int32_t nextScene_ = -1;
};


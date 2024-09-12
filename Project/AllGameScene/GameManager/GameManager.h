#pragma once

#include "IGameScene.h"
#include <cstdint>

enum NextCondition {
	//
	ReplayGame,
	//タイトルに戻る
	ReturnToTitle,
};


int32_t GameManager::nextSceneFromResultScene_ = 0;


//main.cppにあるものを全部こっちに引っ越しする
class GameManager {
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameManager()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// シーンチェンジ
	/// </summary>
	/// <param name="newGameScene"></param>
	void ChangeScene(IGameScene* newGameScene);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void DrawSpriteBack();
	void DrawObject3D();
	void DrawSprite();

	/// <summary>
	/// ポストエフェクト
	/// </summary>
	void PreDrawPostEffectFirst();
	void DrawPostEffect();
	



	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameManager();

private:

	//StatePatternに必要な変数
	IGameScene* currentGamaScene_ = nullptr;
	//シーンの数
	uint32_t SCENE_QUANTITY_ = 0;



public:

	//今回の制作だけ
	//Resuktから次はどのシーンへ行くか
	static int32_t nextSceneFromResultScene_;


	
};
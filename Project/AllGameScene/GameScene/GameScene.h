#pragma once
#include "IGameScene.h"

#include <memory>

#include "Sprite.h"

#include "Model.h"
#include "Camera.h"
#include <BackText.h>


#include "WorldTransform.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include <StageObject/Ground/Groud.h>
#include <DraftPlayer/DraftPlayer.h>
#include <Enemy/Enemy.h>

//StatePatternを使う時は必ず前方宣言をするように
class GameManager;

class GameScene : public IGameScene {
public:

	//コンストラクタ
	GameScene() = default;


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
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();




private:

	//カメラ
	Camera camera_ = {};
	Vector3 cameraPosition_ = {};

	//平行光源
	DirectionalLight directtionalLight_ = {};



	//ポストエフェクト
	//今は使わない
	std::unique_ptr<BackText> back_ = nullptr;



	//プレイヤーの下書き
	std::unique_ptr<DraftPlayer> draftPlayer_ = nullptr;
	//プレイヤーの向いている方向
	Vector3 playerDirection_ = {};






	std::unique_ptr<Enemy> enemy_ = nullptr;




	//地面
	std::unique_ptr<Ground>ground_ = nullptr;


};



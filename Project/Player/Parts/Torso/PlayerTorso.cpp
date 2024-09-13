#include "PlayerTorso.h"



// コピーコンストラクタ
PlayerTorso::PlayerTorso(uint32_t handle)
{
	this->modelHandle_ = handle;
}


// 初期化
void PlayerTorso::Init()
{
	// モデルの初期化
	model_.reset(Model::Create(modelHandle_));

	// トランスフォームの初期化
	transform_.Initialize();

	// マテリアルの初期化
	mtl_.Initialize();
}


// 更新処理
void PlayerTorso::Update()
{
	// トランスフォームの更新
	transform_.Update();

	// マテリアルの更新
	mtl_.Update();
}


// 描画処理
void PlayerTorso::Draw3D(Camera& camera, DirectionalLight& light)
{
	model_->Draw(transform_, camera, mtl_, light);
}

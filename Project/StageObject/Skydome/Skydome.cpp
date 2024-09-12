#include "Skydome.h"

void Skydome::Initialize(uint32_t modelhandle){
	//モデルの生成
	model_.reset(Model::Create(modelhandle));


	//初期化
	worldTransform_.Initialize();
	const float SIZE = 100.0f;
	worldTransform_.scale_ = { .x = SIZE,.y = SIZE,.z = SIZE };
	material_.Initialize();





}

void Skydome::Update(){


	//更新
	worldTransform_.Update();
	material_.Update();
}

void Skydome::Draw(Camera& camera, DirectionalLight& directionalLight){
	//描画
	model_->Draw(worldTransform_, camera, material_, directionalLight);


}

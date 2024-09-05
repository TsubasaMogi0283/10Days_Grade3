#include "Groud.h"
#include "ModelManager.h"


void Ground::Initialize(uint32_t modelhandle){
	model_.reset(Model::Create(modelhandle));


	//初期化
	worldTransform_.Initialize();
	const float SIZE = !00.0f;
	worldTransform_.scale_ = { .x = SIZE,.y = SIZE,.z = SIZE };
	material_.Initialize();

}

void Ground::Update(){


	//更新
	worldTransform_.Update();
	material_.Update();
}

void Ground::Draw(Camera& camera, DirectionalLight& directionalLight){
	//描画
	model_->Draw(worldTransform_, camera, material_, directionalLight);
}

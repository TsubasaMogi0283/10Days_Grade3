#include "Groud.h"
#include "ModelManager.h"


void Ground::Initialize(uint32_t modelhandle){
	//モデルの生成
	model_.reset(Model::Create(modelhandle));


	//初期化
	worldTransform_.Initialize();
	const float SIZE = 100.0f;
	worldTransform_.scale_ = { .x = SIZE,.y = SIZE,.z = SIZE };
	material_.Initialize();





	//左奥
	leftTop_ = {.x= -SIZE,.y=0.0f,.z= SIZE };
	//右奥
	rightTop_ = { .x = SIZE,.y = 0.0f,.z = SIZE };
	//左前
	leftBottom_ = { .x = -SIZE,.y = 0.0f,.z = -SIZE };
	//右前
	rightBottom_ = { .x = SIZE,.y = 0.0f,.z = -SIZE };


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

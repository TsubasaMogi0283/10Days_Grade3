#include "Groud.h"
#include "ModelManager.h"


void Ground::Initialize(uint32_t modelhandle){
	//モデルの生成
	model_.reset(Model::Create(modelhandle));


	//初期化
	worldTransform_.Initialize();
	const float SIZE_ = 100.0f;
	const float scele = 8.0f;
	worldTransform_.scale_ = { .x = scele,.y = scele,.z = scele };
	material_.Initialize();





	//左奥
	leftBack_ = {.x= -SIZE_,.y=0.0f,.z= SIZE_ };
	//右奥
	rightBack_ = { .x = SIZE_,.y = 0.0f,.z = SIZE_ };
	//左前
	leftFront_ = { .x = -SIZE_,.y = 0.0f,.z = -SIZE_ };
	//右前
	rightFront_ = { .x = SIZE_,.y = 0.0f,.z = -SIZE_ };


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

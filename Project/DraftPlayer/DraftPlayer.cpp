#include "DraftPlayer.h"
#include <VectorCalculation.h>

void DraftPlayer::Initialize(uint32_t modelhandle){
	//モデルの生成
	model_.reset(Model::Create(modelhandle));


	//初期化
	worldTransform_.Initialize();
	const float SIZE = 1.0f;
	worldTransform_.scale_ = { .x = SIZE,.y = SIZE,.z = SIZE };
	material_.Initialize();
}

void DraftPlayer::Update(){


#ifdef _DEBUG
	ImGui::Begin("DraftPlayer"); 
	ImGui::Checkbox("IsJump", &isJump_);
	ImGui::InputFloat("Velocity", &velocityY);
	ImGui::End();
#endif // _DEBUG




	//ジャンプするとき
	if (isJump_ == true) {

		
		float acceleationY = -0.5f;

		velocityY += acceleationY;
		
		worldTransform_.translate_.y = velocityY;
		//埋もれないようにする
		if (worldTransform_.translate_.y < worldTransform_.scale_.y) {
			isJump_ = false;
		}

	}
	else{
		velocityY = 10.0f;
		worldTransform_.translate_.y = worldTransform_.scale_.y;
	}



	//方向成分*大きさ
	const float SPEED = 0.5f;
	//加算
	worldTransform_.translate_ = VectorCalculation::Add(worldTransform_.translate_, VectorCalculation::Multiply(direction_, SPEED));





	//更新
	worldTransform_.Update();
	material_.Update();
}

void DraftPlayer::Draw(Camera& camera, DirectionalLight& directionalLight){
	//描画
	model_->Draw(worldTransform_, camera, material_, directionalLight);
}

Vector3 DraftPlayer::GetWorldPosition() const{
	Vector3 position = {
		.x = worldTransform_.worldMatrix_.m[3][0],
		.y = worldTransform_.worldMatrix_.m[3][1],
		.z = worldTransform_.worldMatrix_.m[3][2],
	};

	return position;
}

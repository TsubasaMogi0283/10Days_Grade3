#include "Enemy.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include <VectorCalculation.h>


void Enemy::Initialize(uint32_t& modelHandle, Vector3& position){
	//モデルの生成
	model_.reset(Model::Create(modelHandle));

	//初期化
	worldTransform_.Initialize();
	worldTransform_.translate_ = position;
	material_.Initialize() ;
	speed_ = { -0.0f,0.0f,-0.1f };

	aabb_.max = { .x = position.x + radius_,.y = position.y + radius_,.z = position.z + radius_ };
	aabb_.min = { .x = position.x - radius_,.y = position.x - radius_,.z = position.x - radius_ };

}

void Enemy::Update() {


	//状態
	switch (condition_) {
		//何もしない
	case EnemyCondition::NoneMove:
#ifdef _DEBUG
		ImGui::Begin("None");
		ImGui::End();
#endif // DEBUG

		t_ = 0.0f;
		attackTime_ = 0;
		preTrackingPlayerPosition_ = {};
		preTrackingPosition_ = {};
		speed_ = { 0.0f,0.0f,0.0f };
		break;

		//通常の動き
	case EnemyCondition::Move:



		attackTime_ = 0;
		isAttack_ = false;




#ifdef _DEBUG
		ImGui::Begin("Move");
		ImGui::End();
#endif // DEBUG
		//通常の動き
		t_ = 0.0f;
		preTrackingPlayerPosition_ = {};
		preTrackingPosition_ = {};
		if (speed_.x != 0.0f ||
			speed_.y != 0.0f ||
			speed_.z != 0.0f) {
			direction_ = VectorCalculation::Normalize(speed_);
		}

		worldTransform_.translate_ = VectorCalculation::Add(worldTransform_.translate_, speed_);


		break;

	case EnemyCondition::PreTracking:

		attackTime_ = 0;
		isAttack_ = false;
#pragma region 追跡準備

#ifdef _DEBUG
		ImGui::Begin("PreTracking");
		ImGui::End();
#endif // DEBUG

		//取得したら追跡
		preTrackingPlayerPosition_ = playerPosition_;
		preTrackingPosition_ = GetWorldPosition();


#pragma endregion


		//強制的に追跡
		preCondition_ = EnemyCondition::PreTracking;
		condition_ = EnemyCondition::Tracking;

		break;

		//追跡
	case EnemyCondition::Tracking:
		//追跡処理


#ifdef _DEBUG
		ImGui::Begin("Tracking");
		ImGui::End();
#endif // DEBUG

		//線形補間で移動する
		t_ += 0.005f;
		worldTransform_.translate_ = VectorCalculation::Lerp(preTrackingPosition_, preTrackingPlayerPosition_, t_);


		//向きを求める
		direction_ = VectorCalculation::Subtract(preTrackingPlayerPosition_, preTrackingPosition_);
		direction_ = VectorCalculation::Normalize(direction_);




		break;

		//攻撃
	case EnemyCondition::Attack:
		attackTime_ += 1;


		//2～4秒までが攻撃
		if (attackTime_ > 120 && attackTime_ <= 240) {
			if (attackTime_ == 121) {
				isAttack_ = true;

			}
			else {
				isAttack_ = false;
			}


#ifdef _DEBUG
			ImGui::Begin("Attack");
			ImGui::End();
#endif // DEBUG

		}
		else {
			isAttack_ = false;
		}

		//4秒経ったらまた0になる
		if (attackTime_ > 240) {
			attackTime_ = 0;

		}

		break;

	}


	//加算
	//const float SPEED_SIZE = 0.05f;
	//Vector3 speed = VectorCalculation::Multiply(direction_, SPEED_SIZE);
	//worldTransform_.translate_ = VectorCalculation::Add(worldTransform_.translate_, speed);





	//ワールドトランスフォーム
	worldTransform_.Update();

	//マテリアル
	material_.Update();


	//AABB
	aabb_.min = VectorCalculation::Subtract(GetWorldPosition(), { .x = radius_, .y = radius_, .z = radius_ });
	aabb_.max = VectorCalculation::Add(GetWorldPosition(), {.x = radius_, .y = radius_, .z =radius_});



}

void Enemy::Draw(Camera& camera, DirectionalLight& directionalLight){
	//描画
	model_->Draw(worldTransform_, camera, material_, directionalLight);


}

Vector3 Enemy::GetWorldPosition() const{
	Vector3 position = {
		.x = worldTransform_.worldMatrix_.m[3][0],
		.y = worldTransform_.worldMatrix_.m[3][1],
		.z = worldTransform_.worldMatrix_.m[3][2],
	};
	
	return position;
}

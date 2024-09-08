#include "Enemy.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include <VectorCalculation.h>
#include <Collider/CollisionConfig.h>


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



	isAlive_ = true;

	//種類
	collisionType_ = CollisionType::AABBType;



	//半径
	radius_ = 1.0f;

	//AABBのmax部分に加算する縦横高さのサイズ
	upSideSize_ = { .x = radius_ ,.y = radius_ ,.z = radius_ };

	//AABBのmin部分に加算する縦横高さのサイズ
	downSideSize_ = { .x = radius_ ,.y = radius_ ,.z = radius_ };

	//判定
	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_NONE);




	//攻撃
	uint32_t debugModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	attackModel_ = new EnemyAttackCollision();
	attackModel_->Initialize(debugModelHandle);
	isAttack_ = false;




}

void Enemy::Update() {

	const float SPEED_AMOUNT = 0.2f;
	//状態
	switch (condition_) {
		//何もしない
	case EnemyCondition::NoneMove:
#ifdef _DEBUG
		ImGui::Begin("None");
		ImGui::End();
#endif // DEBUG

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
		preTrackingPlayerPosition_ = {};
		preTrackingPosition_ = {};
		if (speed_.x != 0.0f ||
			speed_.y != 0.0f ||
			speed_.z != 0.0f) {
			direction_ = VectorCalculation::Normalize(speed_);
		}

		worldTransform_.translate_ = VectorCalculation::Add(worldTransform_.translate_, speed_);
		worldTransform_.translate_.y = 1.0f;

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


#ifdef _DEBUG
		ImGui::Begin("Tracking");
		ImGui::End();
#endif // DEBUG

		

		//向きを求める
		direction_ = VectorCalculation::Subtract(preTrackingPlayerPosition_, preTrackingPosition_);
		direction_ = VectorCalculation::Normalize(direction_);

		//加算
		Vector3 speedVelocity = VectorCalculation::Multiply(direction_, SPEED_AMOUNT);
		worldTransform_.translate_ = VectorCalculation::Add(worldTransform_.translate_, speedVelocity);
		worldTransform_.translate_.y = 1.0f;


		break;

		//攻撃
	case EnemyCondition::Attack:
		attackTime_ += 1;


		const int RESET_TIME = 200;

		//2～4秒までが攻撃
		if (attackTime_ > 120 && attackTime_ <= RESET_TIME) {
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
		if (attackTime_ > RESET_TIME) {
			attackTime_ = 0;
		}

		break;

	}


	//倒された処理
	Killed();


	//ワールドトランスフォーム
	worldTransform_.Update();

	//マテリアル
	material_.Update();


	//AABB
	aabb_.min = VectorCalculation::Subtract(GetWorldPosition(), { .x = radius_, .y = radius_, .z = radius_ });
	aabb_.max = VectorCalculation::Add(GetWorldPosition(), {.x = radius_, .y = radius_, .z =radius_});

	Vector3 enemyWorldPosition = GetWorldPosition();
	attackModel_->SetEnemyPosition(enemyWorldPosition);
	attackModel_->SetEnemyDirection(direction_);
	attackModel_->Update();

}

void Enemy::Draw(Camera& camera, DirectionalLight& directionalLight){
	//描画
	if (isAlive_ == true) {
		model_->Draw(worldTransform_, camera, material_, directionalLight);

	}
	

	//攻撃用
	if (isAttack_ == true) {
		attackModel_->Draw(camera, directionalLight);
	}
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 position = {
		.x = worldTransform_.worldMatrix_.m[3][0],
		.y = worldTransform_.worldMatrix_.m[3][1],
		.z = worldTransform_.worldMatrix_.m[3][2],
	};

	return position;
}

void Enemy::OnCollision(){
	isAlive_ = false;
	
}

void Enemy::Killed(){
	//倒された場合
	if (isAlive_ == false) {
		deleteTime_+=1;
	}


	//消える
	if (deleteTime_ > 60 * 2) {
		isKilled_= true;
	}



}


Enemy::~Enemy() {
	delete attackModel_;
}


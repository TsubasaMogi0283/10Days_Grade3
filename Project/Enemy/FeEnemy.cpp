#include "FeEnemy.h"
#include "RockEnemy.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include <VectorCalculation.h>
#include <Collider/CollisionConfig.h>
#include <numbers>
#include "ModelManager.h"

void FeEnemy::Initialize(uint32_t& modelHandle, Vector3& position, Vector3& speed) {
	record_ = Record::GetInstance();

	//モデルの生成
	model_.reset(Model::Create(modelHandle));

	//初期化
	worldTransform_.Initialize();
	worldTransform_.translate_ = position;
	material_.Initialize();
	speed_ = speed;


	//半径
	radius_ = 2.0f;


	isAlive_ = true;

	//種類
	collisionType_ = CollisionType::SphereType;




	//判定
	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER_ATTACK);




	//攻撃
	uint32_t debugModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	attackCollision_ = new EnemyAttackCollision();
	attackCollision_->Initialize(debugModelHandle);
	isAttack_ = false;



	killAudio_ = Audio::GetInstance();
	killHandle_ = killAudio_->LoadWave("Resources/Audio/Game/Fe.wav");
	killAudio_->ChangeVolume(killHandle_, 0.7f);
}

void FeEnemy::Update() {

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
		worldTransform_.translate_.y = 0.0f;

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
		worldTransform_.translate_.y = 0.0f;


		break;

		//攻撃
	case EnemyCondition::Attack:
		attackTime_ += 1;


		const int RESET_TIME = 120;

		//2～4秒までが攻撃
		if (attackTime_ > 40 && attackTime_ <= RESET_TIME) {
			if (attackTime_ == 41) {
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

	//向きを計算しモデルを回転させる
	float directionToRotateY = std::atan2f(-direction_.z, direction_.x);

	const float ROTATE_OFFSET = std::numbers::pi_v<float> / 2.0f;
	ROTATE_OFFSET;
	worldTransform_.rotate_.y = directionToRotateY ;

	//倒された処理
	Killed();


	//ワールドトランスフォーム
	worldTransform_.Update();

	//マテリアル
	material_.Update();



	for (FeEnemyParticle* particle : feParticles_) {
		particle->Update();
	}


	Vector3 enemyWorldPosition = GetWorldPosition();
	attackCollision_->SetEnemyPosition(enemyWorldPosition);
	attackCollision_->SetEnemyDirection(direction_);
	attackCollision_->Update();

}

void FeEnemy::Draw(Camera& camera, DirectionalLight& directionalLight) {
	//描画
	if (isAlive_ == true) {
		model_->Draw(worldTransform_, camera, material_, directionalLight);

	}

	//パーティクルの描画
	if (isDisplayParticle_ == true) {
		for (FeEnemyParticle* particle : feParticles_) {
			particle->Draw(camera, directionalLight);
		}
	}

#ifdef _DEBUG
	//攻撃用
	if (isAttack_ == true) {
		attackCollision_->Draw(camera, directionalLight);
	}
#endif
}

Vector3 FeEnemy::GetWorldPosition() {
	Vector3 position = {
		.x = worldTransform_.worldMatrix_.m[3][0],
		.y = worldTransform_.worldMatrix_.m[3][1],
		.z = worldTransform_.worldMatrix_.m[3][2],
	};

	return position;
}

void FeEnemy::OnCollision() {
	isAlive_ = false;

#ifdef _DEBUG
	ImGui::Begin("FeEnemyOnCollision");
	ImGui::End();
#endif // _DEBUG
}

void FeEnemy::Killed() {
	//倒された場合
	if (isAlive_ == false) {
		deleteTime_ += 1;
		//放出
		if (deleteTime_ == 1) {
			killAudio_->PlayWave(killHandle_, false);

			//パーティクルを生成
			ReleaseParticle();
			//点数を加算
			record_->AddFeScore();
		}
		
		//表示
		isDisplayParticle_ = true;


	}


	//消える
	if (deleteTime_ > 60 * 2) {
		isKilled_ = true;
		isDisplayParticle_ = false;
	}



}

void FeEnemy::ReleaseParticle() {

	FeEnemyParticle* rockParticle = new FeEnemyParticle();
	uint32_t particleModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/Game/Enemy/FeEnemy","FeBreak.obj");
	Vector3 enemyPosition = GetWorldPosition();
	rockParticle->Initialize(particleModelHandle, enemyPosition);
	feParticles_.push_back(rockParticle);
}


FeEnemy::~FeEnemy() {
	for (FeEnemyParticle* particle : feParticles_) {
		delete particle;
	}

	delete attackCollision_;
}


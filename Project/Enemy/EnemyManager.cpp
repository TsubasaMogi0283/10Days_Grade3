#include "EnemyManager.h"
#include <VectorCalculation.h>
#include <SingleCalculation.h>

#include "ModelManager.h"

#include "RockEnemy.h"
#include "FeEnemy.h"
#include <random>

void EnemyManager::Initialize(uint32_t& rockModelhandle, uint32_t& feModelHandle){
	
	feModelHandle;

	for (int i = 0; i < 10; ++i) {
		//岩
		GenarateRockEnemy(rockModelhandle);
		//鉄
		//GenarateFeEnemy(feModelHandle);

	}
	
	enemyes_;


}




void EnemyManager::DeleteEnemy(){
	//敵が生存していなかったら消す
	enemyes_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsKilled() == true) {
			delete enemy;
			return true;
		}
		return false;
	});
}

void EnemyManager::GenarateRockEnemy(uint32_t& rockModelhandle){
	Enemy* rockEnemy1 = new RockEnemy();
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distributeX(stageLeftBackPosition.x, stageRightBackPosition.x);
	std::uniform_real_distribution<float> distributeZ(stageLeftFrontPosition.z, stageLeftBackPosition.z);
	
	//ランダムの値
	Vector3 randomTranslate = { distributeX(randomEngine),0.0f,distributeZ(randomEngine) };

	Vector3 enemyPosition1 = randomTranslate;
	rockEnemy1->Initialize(rockModelhandle, enemyPosition1);
	enemyes_.push_back(rockEnemy1);


}

void EnemyManager::GenarateFeEnemy(uint32_t& feModelHandle){
	Enemy* feEnemy2 = new FeEnemy();
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distributeX(stageLeftBackPosition.x, stageRightBackPosition.x);
	std::uniform_real_distribution<float> distributeZ(stageLeftBackPosition.z, stageLeftFrontPosition.z);

	//ランダムの値
	Vector3 randomTranslate = { distributeX(randomEngine),0.0f,distributeZ(randomEngine) };

	Vector3 enemyPosition2 = randomTranslate;
	feEnemy2->Initialize(feModelHandle, enemyPosition2);
	enemyes_.push_back(feEnemy2);

}




void EnemyManager::Tracking(){


	//接近するときの距離
	const float TRACKING_START_DISTANCE_ = 40.0f;
	////攻撃するときの距離
	const float ATTACK_START_DISTANCE_ = 4.0f;



	//現在の敵の数
	uint32_t enemyQuantity = static_cast<uint32_t>(enemyes_.size());

	//プレイヤー
	Vector3 playerSize = { .x = 1.0f,.y = 1.0f,.z = 1.0f };
	Vector3 playerMaxAABB = { VectorCalculation::Add(playerPosition_,playerSize) };
	Vector3 playerMinAABB = { VectorCalculation::Subtract(playerPosition_,playerSize) };
	AABB playerAABB = { .min = playerMinAABB , .max = playerMaxAABB };


	if (enemyQuantity >= 1u) {
		for (Enemy* enemy : enemyes_) {

			//AABBを取得
			AABB enemyAABB = enemy->GetAABB();

			//状態
			uint32_t condition = enemy->GetCondition();

			//向き
			Vector3 enemyDirection = enemy->GetDirection();

			//プレイヤーと敵の差分ベクトル
			//対象にするものがプレイヤーなのでプレイヤーから引いてね
			Vector3 defference = VectorCalculation::Subtract(playerPosition_, enemy->GetWorldPosition());



			Vector3 normalizedDefference = VectorCalculation::Normalize(defference);
			float dot = SingleCalculation::Dot(normalizedDefference, enemyDirection);




			//距離を求める
			float distance = SingleCalculation::Length(defference);

			//前方にいる時だけ追跡する
			if (dot > 0.8f) {

				
				if ((distance < TRACKING_START_DISTANCE_&&distance>ATTACK_START_DISTANCE_)
					&&condition==EnemyCondition::Move) {

					//前の状態を保存
					enemy->SetPreCondition(condition);

					//現在の状態を保存
					uint32_t newCondition = EnemyCondition::PreTracking;
					enemy->SetPlayerPosition(playerPosition_);
					enemy->SetCondition(newCondition);
				}
				
				//追跡している時
				if (distance<= ATTACK_START_DISTANCE_&&condition == EnemyCondition::Tracking) {
					//前の状態を保存
					enemy->SetPreCondition(condition);

					//現在の状態を保存
					uint32_t newCondition = EnemyCondition::Attack;
					enemy->SetCondition(newCondition);
				}
				//攻撃中にプレイヤーが離れた時
				if (distance > ATTACK_START_DISTANCE_ && condition == EnemyCondition::Attack) {
					//前の状態を保存
					enemy->SetPreCondition(condition);

					//現在の状態を保存
					uint32_t newCondition = EnemyCondition::Move;
					enemy->SetCondition(newCondition);
				}





			}
			//前方にいなければ強制的にMove
			else {
				//前の状態を保存
				enemy->SetPreCondition(condition);
				//現在の状態を保存
				uint32_t newCondition = EnemyCondition::Move;
				enemy->SetCondition(newCondition);
			}

#ifdef _DEBUG
			ImGui::Begin("OneEnemy"); 
			int intCondition = static_cast<int>(condition);
			ImGui::InputInt("状態", &intCondition);
			ImGui::InputFloat("内積", &dot);
			ImGui::InputFloat3("敵の向き", &enemyDirection.x);
			ImGui::InputFloat3("差分", &defference.x);

			ImGui::End();
#endif // _DEBUG

		}
	}

}


void EnemyManager::Update(){

	
	for (Enemy* enemy : enemyes_) {

		//AABBを取得
		AABB enemyAABB = enemy->GetAABB();

		//X反転
		if ((enemyAABB.min.x < stageLeftBackPosition.x) || (enemyAABB.max.x > stageRightBackPosition.x)) {
			enemy->InvertSpeedX();
		}
		//Z反転
		if ((enemyAABB.min.z < stageLeftFrontPosition.z) || (enemyAABB.max.z > stageLeftBackPosition.z)) {
			enemy->InvertSpeedZ();
		}

		//敵の更新
		enemy->Update();
	}

	//追跡
	Tracking();



}

void EnemyManager::Draw(Camera& camera, DirectionalLight& directionalLight){
	//敵の描画
	for (Enemy* enemy : enemyes_) {
		enemy->Draw(camera, directionalLight);
	}

}

EnemyManager::~EnemyManager(){
	for (Enemy* enemy : enemyes_) {
		delete enemy;
	}
}






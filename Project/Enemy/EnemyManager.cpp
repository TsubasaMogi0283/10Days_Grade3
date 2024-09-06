#include "EnemyManager.h"
#include <VectorCalculation.h>
#include <SingleCalculation.h>

void EnemyManager::Initialize(uint32_t& modelhandle){


#pragma region 通常の敵の生成

	Enemy* enemy1 = new Enemy();
	Vector3 enemyPosition1 = { 10.0f,0.0f,1.0f };
	enemy1->Initialize(modelhandle, enemyPosition1);
	enemyes_.push_back(enemy1);



	Enemy* enemy2 = new Enemy();
	Vector3 enemyPosition2 = { 1.0f,0.0f,10.0f };
	enemy2->Initialize(modelhandle, enemyPosition2);
	enemyes_.push_back(enemy2);


#pragma endregion

}




void EnemyManager::DeleteEnemy(){
	//敵が生存していなかったら消す
	enemyes_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsAlive() == false) {
			delete enemy;
			return true;
		}
		return false;
	});
}




void EnemyManager::Tracking(){


	//接近するときの距離
	const float TRACKING_START_DISTANCE_ = 15.0f;
	//攻撃するときの距離
	const float ATTACK_START_DISTANCE_ = 6.0f;

	const float MINIMUM_DISTANCE = 2.0f;



	//追跡
	for (std::list<Enemy*>::iterator it1 = enemyes_.begin(); it1 != enemyes_.end(); ++it1) {


		
		//AABB
		aabb[0] = (*it1)->GetAABB();

		enemyPosition[0] = (*it1)->GetWorldPosition();
		//向き
		Vector3 direction = (*it1)->GetDirection();

		//内積
		float dot = 0.0f;

		for (std::list<Enemy*>::iterator it2 = enemyes_.begin(); it2 != enemyes_.end(); ++it2) {

			//it1とit2が一致した場合は計算をせずに次のループへ
			if (it1 == it2) {
				continue;
			}

			//2体目のAABBを取得
			aabb[1] = (*it2)->GetAABB();

			//接触している場合
			if ((aabb[0].min.x < aabb[1].max.x && aabb[0].max.x > aabb[1].min.x) &&
				(aabb[0].min.y < aabb[1].max.y && aabb[0].max.y > aabb[1].min.y) &&
				(aabb[0].min.z < aabb[1].max.z && aabb[0].max.z > aabb[1].min.z)) {
				//ワールド座標
				enemyPosition[1] = (*it2)->GetWorldPosition();


				//敵同士の差分ベクトル
				Vector3 enemyAndEnemyDifference = VectorCalculation::Subtract(enemyPosition[1], enemyPosition[0]);

				//内積
				//進行方向の前にいると+
				Vector3 normalizedEnemyAndEnemy = VectorCalculation::Normalize(enemyAndEnemyDifference);
				dot = SingleCalculation::Dot(direction, normalizedEnemyAndEnemy);


				break;
			}



		}

		//現在の状態
		uint32_t condition = (*it1)->GetCondition();

		//プレイヤーとの距離
		Vector3 playerEnemyDifference = VectorCalculation::Subtract(playerPosition_, (*it1)->GetWorldPosition());
		float playerEnemyDistance = SingleCalculation::Length(playerEnemyDifference);


		//前方にいた場合
		if (dot > 0.0f && condition == EnemyCondition::Move) {
			//接触した場合
			//止まる
			if ((aabb[0].min.x < aabb[1].max.x && aabb[0].max.x > aabb[1].min.x) &&
				(aabb[0].min.y < aabb[1].max.y && aabb[0].max.y > aabb[1].min.y) &&
				(aabb[0].min.z < aabb[1].max.z && aabb[0].max.z > aabb[1].min.z)) {
				uint32_t newCondition = EnemyCondition::NoneMove;

				//前の状態を保存
				(*it1)->SetPreCondition(condition);
				//状態の変更
				(*it1)->SetCondition(newCondition);

				continue;
			}
			//接触していない場合
			else {
				uint32_t newCondition = EnemyCondition::Move;

				//前の状態を保存
				(*it1)->SetPreCondition(condition);
				//状態の変更
				(*it1)->SetCondition(newCondition);
			}
		}
		//前方にいない場合
		else {
			if (playerEnemyDistance > TRACKING_START_DISTANCE_) {
				uint32_t newCondition = EnemyCondition::Move;

				//前の状態を保存
				(*it1)->SetPreCondition(condition);
				//スピードの保存
				//(*it1)->SaveSpeed();
				//状態の変更
				(*it1)->SetCondition(newCondition);
			}


			//設定した値より短くなったら接近開始
			if (condition == EnemyCondition::Move) {
				if (playerEnemyDistance <= TRACKING_START_DISTANCE_) {
					//前回のMove状態を記録
					uint32_t preCondition = condition;
					(*it1)->SetPreCondition(preCondition);

					//状態を記録
					condition = EnemyCondition::PreTracking;
					(*it1)->SetCondition(condition);
				}
			}

			//追跡の時に
			if (condition == EnemyCondition::Tracking) {
				//Moveへ
				if (playerEnemyDistance > TRACKING_START_DISTANCE_) {
					//前回のMove状態を記録
					uint32_t preCondition = condition;
					(*it1)->SetPreCondition(preCondition);

					//状態を記録
					condition = EnemyCondition::Move;
					(*it1)->SetCondition(condition);

				}

				//設定した値より短くなったら攻撃開始
				if (playerEnemyDistance <= ATTACK_START_DISTANCE_ &&
					MINIMUM_DISTANCE < playerEnemyDistance) {
					//前回のMove状態を記録
					uint32_t preCondition = condition;
					(*it1)->SetPreCondition(preCondition);

					//状態を記録
					condition = EnemyCondition::Attack;
					(*it1)->SetCondition(condition);

				}
			}
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









	//現在の敵の数
	uint32_t enemyQuantity = static_cast<uint32_t>(enemyes_.size());
	enemyQuantity;
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






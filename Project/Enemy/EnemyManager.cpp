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
	const float TRACKING_START_DISTANCE_ = 40.0f;
	////攻撃するときの距離
	const float ATTACK_START_DISTANCE_ = 6.0f;
	const float MINIMUM_DISTANCE = 2.0f;



	//現在の敵の数
	uint32_t enemyQuantity = static_cast<uint32_t>(enemyes_.size());



	//プレイヤーの
	Vector3 playerSize = { .x = 1.0f,.y = 1.0f,.z = 1.0f };
	Vector3 playerMaxAABB = { VectorCalculation::Add(playerPosition_,playerSize) };
	Vector3 playerMinAABB = { VectorCalculation::Subtract(playerPosition_,playerSize) };
	AABB playerAABB = { .min = playerMinAABB , .max = playerMaxAABB };

	//1体だけの時
	if (enemyQuantity == 1u) {
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

				
				if (distance < TRACKING_START_DISTANCE_&&condition==EnemyCondition::Move) {

					//前の状態を保存
					enemy->SetPreCondition(condition);

					//現在の状態を保存
					uint32_t newCondition = EnemyCondition::PreTracking;
					enemy->SetPlayerPosition(playerPosition_);
					enemy->SetCondition(newCondition);
				}
				
			}
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





	//1体より多い時
	if (enemyQuantity > 1u) {
		for (std::list<Enemy*>::iterator it1 = enemyes_.begin(); it1 != enemyes_.end(); ++it1) {

			//AABB
			AABB enemy1AABB = (*it1)->GetAABB();
			AABB enemy2AABB = {};
			//位置1
			Vector3 enemy1Position = (*it1)->GetWorldPosition();
			//向き
			Vector3 direction1 = (*it1)->GetDirection();

			float dot = 0.0f;

			for (std::list<Enemy*>::iterator it2 = enemyes_.begin(); it2 != enemyes_.end(); ++it2) {

				//it1とit2が一致した場合は計算をせずに次のループへ
				if (it1 == it2) {
					continue;
				}

				//2体目のAABBを取得
				enemy2AABB = (*it2)->GetAABB();

				//接触している場合
				if ((enemy1AABB.min.x < enemy2AABB.max.x && enemy1AABB.max.x > enemy2AABB.min.x) &&
					(enemy1AABB.min.y < enemy2AABB.max.y && enemy1AABB.max.y > enemy2AABB.min.y) &&
					(enemy1AABB.min.z < enemy2AABB.max.z && enemy1AABB.max.z > enemy2AABB.min.z)) {
					//ワールド座標
					Vector3 enemy2Position = (*it2)->GetWorldPosition();


					//敵同士の差分ベクトル
					Vector3 enemyAndEnemyDifference = VectorCalculation::Subtract(enemy1Position, enemy2Position);

					//進行方向の前にいると+
					Vector3 normalizedEnemyAndEnemy = VectorCalculation::Normalize(enemyAndEnemyDifference);
					//内積
					dot = SingleCalculation::Dot(direction1, normalizedEnemyAndEnemy);


					break;
				}

			}




			//プレイヤーとの距離
			Vector3 playerEnemyDifference = VectorCalculation::Subtract(playerPosition_, (*it1)->GetWorldPosition());
			float playerEnemyDistance = SingleCalculation::Length(playerEnemyDifference);


			//敵1の現在の状態
			uint32_t condition = (*it1)->GetCondition();


			//前方にいた場合
			if (dot > 0.9f && condition == EnemyCondition::Move) {




				//接触した場合
				//止まる
				if ((enemy1AABB.min.x < enemy2AABB.max.x && enemy1AABB.max.x > enemy2AABB.min.x) &&
					(enemy1AABB.min.y < enemy2AABB.max.y && enemy1AABB.max.y > enemy2AABB.min.y) &&
					(enemy1AABB.min.z < enemy2AABB.max.z && enemy1AABB.max.z > enemy2AABB.min.z)) {
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






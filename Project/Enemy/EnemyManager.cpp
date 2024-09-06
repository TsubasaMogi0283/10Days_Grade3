#include "EnemyManager.h"

void EnemyManager::Initialize(uint32_t& modelhandle){


#pragma region 通常の敵の生成

	Enemy* enemy1 = new Enemy();
	Vector3 enemyPosition = { 1.0f,0.0f,1.0f };
	enemy1->Initialize(modelhandle, enemyPosition);
	enemyes_.push_back(enemy1);

#pragma endregion

}



void EnemyManager::InvertDirection() {
	

	//反転処理
	for (Enemy* enemy : enemyes_) {
		//AABBを取得
		AABB enemyAABB = enemy->GetAABB();

		//X
		if ((enemyAABB.min.x < stageLeftBackPosition.x) || (enemyAABB.max.x > stageRightBackPosition.x)) {
			enemy->InvertSpeedX();
		}
		//Z
		if ((enemyAABB.min.z < stageLeftFrontPosition.z) || (enemyAABB.max.z > stageLeftBackPosition.z)) {
			enemy->InvertSpeedZ();
		}
	}


}


void EnemyManager::Update(){

	//敵の更新
	for (Enemy* enemy : enemyes_) {
		enemy->Update();
	}

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





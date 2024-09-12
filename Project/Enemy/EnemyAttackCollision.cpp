#include "EnemyAttackCollision.h"
#include <Collider/CollisionConfig.h>
#include <VectorCalculation.h>
#include "Camera.h"
#include "DirectionalLight.h"

void EnemyAttackCollision::Initialize(uint32_t& modelHandle){
	model_.reset(Model::Create(modelHandle));

	//半径
	radius_ = 1.5f;

	//初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = { .x = radius_,.y = radius_,.z = radius_ };
	material_.Initialize();
	material_.lightingKinds_ = Directional;
	material_.color_ = { .x = 1.0f,.y = 1.0f,.z = 1.0f,.w = 1.0f };

	enemyWorldPosition_ = {};


	#pragma region 当たり判定

	//種類
	collisionType_ = CollisionType::SphereType;

	


	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY_ATTACK);
	//相手
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER);

	#pragma endregion

}

void EnemyAttackCollision::Update(){

	
	//間隔
	const float INTERVAL = 5.0f;
	Vector3 newDirection = VectorCalculation::Multiply(enemyDirection_, INTERVAL);
	//更新
	worldTransform_.translate_ = VectorCalculation::Add(enemyWorldPosition_, newDirection);
	worldTransform_.Update();
	material_.Update();

	aabb_.max= VectorCalculation::Add(enemyWorldPosition_, { .x = radius_ ,.y = radius_ ,.z = radius_ });
	aabb_.min = VectorCalculation::Subtract(enemyWorldPosition_, { .x = radius_ ,.y = radius_ ,.z = radius_ });;


}

void EnemyAttackCollision::Draw(Camera& camera, DirectionalLight& spotLight){
	model_->Draw(worldTransform_, camera, material_, spotLight);
}



Vector3 EnemyAttackCollision::GetWorldPosition(){
	Vector3 worldPosition = {
		.x = worldTransform_.worldMatrix_.m[3][0],
		.y = worldTransform_.worldMatrix_.m[3][1],
		.z = worldTransform_.worldMatrix_.m[3][2],
	};
	return worldPosition;
}

void EnemyAttackCollision::OnCollision(){

}


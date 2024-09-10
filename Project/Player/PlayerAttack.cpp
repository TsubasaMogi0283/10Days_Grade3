#include "PlayerAttack.h"
#include "ModelManager.h"
#include <VectorCalculation.h>
#include <Collider/CollisionConfig.h>

void PlayerAttack::Initialize(Vector3& position){
	uint32_t modelHandle= ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	model_.reset(Model::Create(modelHandle));

	collisionType_ = CollisionType::SphereType;
	radius_ = 1.0f;

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = { radius_,radius_,radius_ };
	worldTransform_.translate_ = position;

	//マテリアルの初期化
	material_.Initialize();
	material_.lightingKinds_ = Directional;


	//種類
	collisionType_ = CollisionType::SphereType;


	//判定
	//自分
	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER_ATTACK);
	//敵本体
	SetCollisionMask(COLLISION_ATTRIBUTE_ENEMY);


}

void PlayerAttack::Update(){


	worldTransform_.translate_ = playerPosition_;
	worldTransform_.Update();
	material_.Update();




}

void PlayerAttack::Draw(Camera& camera, DirectionalLight& directionalLight){
	model_->Draw(worldTransform_, camera, material_, directionalLight);
}

void PlayerAttack::OnCollision(){

#ifdef _DEBUG
	ImGui::Begin("PlayerAttackOnCollision");
	ImGui::End();
#endif


}

Vector3 PlayerAttack::GetWorldPosition(){
	return worldTransform_.GetWorldPosition();
}

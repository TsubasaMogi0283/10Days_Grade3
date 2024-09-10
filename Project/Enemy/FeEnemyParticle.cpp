#include "FeEnemyParticle.h"


void FeEnemyParticle::Initialize(uint32_t& modelHandle, Vector3& position) {

	particle_.reset(Particle3D::Create(modelHandle, ThrowUp));
	particle_->SetTranslate(position);
	material_.Initialize();
	material_.lightingKinds_ = Directional;
}

void FeEnemyParticle::Update() {
	material_.Update();
}

void FeEnemyParticle::Draw(Camera& camera, DirectionalLight& directionalLight) {

	//描画
	particle_->Draw(camera, material_, directionalLight);

}

#pragma once
#include <memory>

#include "Particle3D.h"
#include "Material.h"

struct Camera;
struct DirectionalLight;

class EnemyParticle {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="position"></param>
	virtual void Initialize(uint32_t& modelHandle, Vector3& position)=0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(Camera& camera, DirectionalLight& directionalLight)=0;




protected:
	//パーティクル
	std::unique_ptr<Particle3D> particle_ = nullptr;
	//マテリアル
	Material material_ = {};


};
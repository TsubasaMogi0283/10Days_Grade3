#pragma once
#include "EnemyParticle.h"

class FeEnemyParticle :public EnemyParticle {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FeEnemyParticle() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t& modelHandle, Vector3& position)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;


	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Camera& camera, DirectionalLight& directionalLight) override;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FeEnemyParticle() = default;

public:
	void SetModelHandle(uint32_t& modelHandle) {
		this->modelHandle_ = modelHandle;
	}



private:
	uint32_t modelHandle_ = 0u;


};


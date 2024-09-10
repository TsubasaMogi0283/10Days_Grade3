#pragma once
#include "Enemy.h"
#include <list>
#include "FeEnemyParticle.h"


class FeEnemy : public Enemy{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FeEnemy() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="position"></param>
	void Initialize(uint32_t& modelHandle, Vector3& position, Vector3& speed)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="directionalLight"></param>
	void Draw(Camera& camera, DirectionalLight& directionalLight)override;


	/// <summary>
	/// デストラクタ
	/// </summary>
	~FeEnemy()override;



public:
	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()override;

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision()override;

	/// <summary>
	/// 倒された
	/// </summary>
	void Killed();




	/// <summary>
	/// パーティクルの放出
	/// </summary>
	void ReleaseParticle();

private:
	//パーティクル
	std::list<FeEnemyParticle*> feParticles_{};
	bool isDisplayParticle_ = false;

};


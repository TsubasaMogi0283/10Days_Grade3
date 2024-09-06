#pragma once
#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "EnemyAttackCollision.h"
#include "AABB.h"

struct Camera;
struct DirectionalLight;
class DraftPlayer;

//後々StatePatternで分けるつもり
enum EnemyCondition {
	//何もしない
	NoneMove,
	//通常
	Move,
	//追いかけ始めるときの座標
	PreTracking,
	//追いかける
	Tracking,
	//攻撃
	Attack,


};




class Enemy{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="position"></param>
	void Initialize(uint32_t& modelHandle,Vector3 &position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="directionalLight"></param>
	void Draw(Camera &camera,DirectionalLight& directionalLight);


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy() = default;



public:
	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()const;




	/// <summary>
	/// 状態の取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCondition() const{
		return condition_;
	}

	//半径の取得
	float GetRadius()const {
		return radius_;
	}


	inline AABB GetAABB()const {
		return aabb_;
	}


	inline void InvertSpeedX() {
		speed_.x *= -1.0f;
	}
	inline void InvertSpeedZ() {
		speed_.z *= -1.0f;
	}

private:
	//モデル
	std::unique_ptr<Model>model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};

	//マテリアル
	Material material_ = {};



	AABB aabb_ = {};

	//向き
	Vector3 direction_ = {};

	float radius_ = 1.0f;

private:
	DraftPlayer* draftplayer_ = nullptr;
	

	//攻撃用
	EnemyAttackCollision* attackModel_ = nullptr;
	bool isAttack_ = false;



private:

	//移動速度
	Vector3 preSpeed_ = {};
	Vector3 speed_ = {};


	//追跡
	bool isTracking_ = false;
	Vector3 preTrackingPosition_ = {};
	Vector3 preTrackingPlayerPosition_ = {};

	//向き
	//Vector3 direction_ = {};
	float t_ = 0.0f;

	//攻撃
	int32_t attackTime_ = 0;





private:
	//状態
	uint32_t preCondition_ = EnemyCondition::NoneMove;
	uint32_t condition_ = EnemyCondition::Move;




};


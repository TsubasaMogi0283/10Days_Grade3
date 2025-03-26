#pragma once
#include <memory>


#include "WorldTransform.h"
#include "Model.h"
#include "Material.h"
#include <AABB.h>
#include "EnemyAttackCollision.h"
#include "Collider/Collider.h"
#include "Record/Record.h"




struct Camera;
struct DirectionalLight;



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


//基底クラス
//これをもとに敵を作っていく

class Enemy : public Collider {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="position"></param>
	virtual void Initialize(uint32_t& modelHandle, Vector3& position,Vector3 &speed) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update()=0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="directionalLight"></param>
	virtual void Draw(Camera& camera, DirectionalLight& directionalLight) = 0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Enemy() = default;


public:


	/// <summary>
	/// 方向の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetDirection()const {
		return direction_;
	}



	void SetPlayerPosition(Vector3 position) {
		this->playerPosition_ = position;
	}


	/// <summary>
	/// 倒されたかどうかのフラグを取得
	/// </summary>
	/// <returns></returns>
	inline bool GetIsKilled() const {
		return isKilled_;
	}


	/// <summary>
	/// 状態の取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCondition() const {
		return condition_;
	}


	void SetCondition(uint32_t& condition) {
		this->condition_ = condition;
	}

	void SetPreCondition(uint32_t& preCondition) {
		this->preCondition_ = preCondition;
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

	/// <summary>
	/// 衝突判定用の攻撃を取得
	/// </summary>
	/// <returns></returns>
	EnemyAttackCollision* GetEnemyAttackCollision() const{
		return attackCollision_;
	}

	/// <summary>
	/// 攻撃しているかどうか
	/// </summary>
	/// <returns></returns>
	bool GetIsAttack()const {
		return isAttack_;
	}

protected:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//マテリアル
	Material material_ = {};
	//生きているかどうか
	bool isAlive_ = true;

	//向き
	Vector3 direction_ = {};

	Record* record_ = nullptr;


protected:

	//攻撃用
	EnemyAttackCollision* attackCollision_ = nullptr;
	//攻撃しているかどうか
	bool isAttack_ = false;
	//倒されたか
	bool isKilled_ = false;
	//消える
	int deleteTime_ = 0;
	//時間
	int32_t attackTime_ = 0;


protected:

	//移動速度
	Vector3 preSpeed_ = {};
	Vector3 speed_ = {};

	//プレイヤーの座標
	Vector3 playerPosition_ = {};

	//追跡
	bool isTracking_ = false;
	Vector3 preTrackingPosition_ = {};
	Vector3 preTrackingPlayerPosition_ = {};



	

protected:

	//状態
	uint32_t preCondition_ = EnemyCondition::NoneMove;
	uint32_t condition_ = EnemyCondition::Move;


};

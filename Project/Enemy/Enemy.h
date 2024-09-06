#pragma once
#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "EnemyAttackCollision.h"


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
	void Initialize(uint32_t& modelHandle);

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


	//void SetDraftPlayer(DraftPlayer& draftPlayer) {
	//	this->draftplayer_ = draftPlayer;
	//}



private:
	//モデル
	std::unique_ptr<Model>model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};

	//マテリアル
	Material material_ = {};



	//向き
	Vector3 direction_ = {};



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


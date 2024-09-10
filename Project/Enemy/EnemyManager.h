#pragma once

#include <list>


class ModelManager;

#include "Enemy.h"
class EnemyManager{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="rockModelhandle"></param>
	/// <param name="feModelHandle"></param>
	void Initialize(uint32_t& rockModelhandle,uint32_t &feModelHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="directionalLight"></param>
	void Draw(Camera& camera,DirectionalLight& directionalLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyManager();


public:

	/// <summary>
	/// 追跡
	/// </summary>
	void Tracking();

	/// <summary>
	/// プレイヤーの座標を取得
	/// </summary>
	/// <param name="playerPosition"></param>
	void SetPlayerPosition(Vector3& playerPosition) {
		this->playerPosition_ = playerPosition;
	}




public:


	/// <summary>
	/// ステージの四隅の座標を取得
	/// </summary>
	/// <param name="leftBack"></param>
	/// <param name="rightBack"></param>
	/// <param name="leftFront"></param>
	/// <param name="rightFront"></param>
	void SetStageRectPosition(Vector3& leftBack, Vector3& rightBack, Vector3& leftFront, Vector3& rightFront) {
		this->stageLeftBackPosition = leftBack;
		this->stageRightBackPosition = rightBack;
		this->stageLeftFrontPosition = leftFront;
		this->stageRightFrontPosition = rightFront;
	}

	/// <summary>
	/// エネミーを消す処理
	/// </summary>
	void DeleteEnemy();


	//リストの取得
	std::list<Enemy*> GetEnemyList() {
		return enemyes_;
	}


private:
	/// <summary>
	/// 岩の敵
	/// </summary>
	void GenarateRockEnemy(uint32_t& rockModelhandle);

	/// <summary>
	/// 鉄の敵
	/// </summary>
	void GenarateFeEnemy(uint32_t& feModelHandle);


private:
	ModelManager* modelManager_ = nullptr;

	//最大数
	const uint32_t MAX_ENEMY_COUNT_ = 15u;

	uint32_t genarateTime_ = 0u;


	//通常の敵
	std::list<Enemy*> enemyes_ = {};

private:
	//岩
	uint32_t rockEnemyModelHandle_ = 0u;
	//鉄
	uint32_t feEnemyModelHandle_ = 0u;



private:

	//プレイヤーの座標
	Vector3 playerPosition_ = {};

	//ステージの四隅の座標
	//左奥
	Vector3 stageLeftBackPosition = {};
	//右奥
	Vector3 stageRightBackPosition = {};
	//左前
	Vector3 stageLeftFrontPosition = {};
	//右前
	Vector3 stageRightFrontPosition = {};


};


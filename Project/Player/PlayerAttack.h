#pragma once
#include <memory>

#include "Collider/Collider.h"
#include "WorldTransform.h"
#include "Material.h"
#include "Model.h"


struct Camera;
struct DirectionalLight;

class PlayerAttack:public Collider{

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerAttack() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position"></param>
	void Initialize(Vector3& position);


	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="directionalLight"></param>
	void Draw(Camera& camera, DirectionalLight& directionalLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerAttack() = default;

public:

	/// <summary>
	/// 衝突
	/// </summary>
	void OnCollision()override;


	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()override;

	/// <summary>
	/// プレイヤー座標の取得
	/// </summary>
	/// <param name="position"></param>
	void SetPlayerPosition(Vector3 &position) {
		this->playerPosition_ = position;
	}

private:
	//モデル
	std::unique_ptr<Model>model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};
	//マテリアル
	Material material_ = {};

	//プレイヤー座標
	Vector3 playerPosition_ = {};


};


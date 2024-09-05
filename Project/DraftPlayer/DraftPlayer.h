#pragma once
#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"




class DraftPlayer{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DraftPlayer() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t modelhandle);

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
	~DraftPlayer() = default;


public:
	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()const;

	/// <summary>
	/// 方向の入力
	/// </summary>
	inline void SetPlayerDirection(Vector3& direction) {
		this->direction_ = direction;
	}




	void SetIsJump(bool isJump) {
		this->isJump_ = isJump;
	}

private:
	//モデル
	std::unique_ptr<Model>model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};

	//マテリアル
	Material material_ = {};



	//方向
	Vector3 direction_ = {};


	//ジャンプしているかどうか
	bool isJump_ = false;




	float velocityY = 10.0f;

};


#pragma once
#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"


struct Camera;
struct DirectionalLight;

class Ground{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Ground() = default;

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
	~Ground() = default;



public:
	/// <summary>
	/// 左奥
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetLeftTop()const {
		return leftTop_;
	}


	/// <summary>
	/// 左奥
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetRightTop()const {
		return rightTop_;
	}

	/// <summary>
	/// 左前
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetLeftBottom()const {
		return leftBottom_;
	}

	/// <summary>
	/// 右前
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetRightBottom()const {
		return rightBottom_;
	}

private:
	//モデル
	std::unique_ptr<Model>model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};

	//マテリアル
	Material material_ = {};


	//左奥
	Vector3 leftTop_ = {};
	//右奥
	Vector3 rightTop_ = {};
	//左前
	Vector3 leftBottom_ = {};
	//右前
	Vector3 rightBottom_ = {};


};


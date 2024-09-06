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
	inline Vector3 GetLeftBack()const {
		return leftBack_;
	}


	/// <summary>
	/// 左奥
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetRightBack()const {
		return rightBack_;
	}

	/// <summary>
	/// 左前
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetLeftFront()const {
		return leftFront_;
	}

	/// <summary>
	/// 右前
	/// </summary>
	/// <returns></returns>
	inline Vector3 GetRightFront()const {
		return rightFront_;
	}

private:
	//モデル
	std::unique_ptr<Model>model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};

	//マテリアル
	Material material_ = {};


	//左奥
	Vector3 leftBack_ = {};
	//右奥
	Vector3 rightBack_ = {};
	//左前
	Vector3 leftFront_ = {};
	//右前
	Vector3 rightFront_ = {};


};


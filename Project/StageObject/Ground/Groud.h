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

private:
	//モデル
	std::unique_ptr<Model>model_ = nullptr;
	WorldTransform worldTransform_ = {};
	Material material_ = {};

};


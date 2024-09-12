#pragma once

#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include <LevelDataManager.h>


struct Camera;
struct DirectionalLight;


class Skydome{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Skydome() = default;

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
	~Skydome() = default;




private:
	//モデル
	std::unique_ptr<Model>model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};

	//マテリアル
	Material material_ = {};




};


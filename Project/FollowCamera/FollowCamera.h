#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "Input.h"
#include "VectorCalculation.h"
#include "Matrix4x4Calculation.h"
#include "Camera.h"


// 前方宣言
class Player;
struct Camera;
struct DirectionalLight;


/* フォローカメラ */
class FollowCamera {
	
public:

	// コンストラクタ、デストラクタ
	FollowCamera() = default;
	~FollowCamera() = default;

	// コピーコンストラクタ
	FollowCamera(uint32_t modelHandle);

	// 初期化、更新、描画
	void Init();
	void Update();
	void Draw3D(Camera& camera, DirectionalLight& light);

	// stick入力時の処理
	void FuncStickFunc(XINPUT_STATE joyState);


#pragma region Accessor アクセッサ

	// Player
	void SetPlayer(Player* player) { this->player_ = player; }

	// Camera
	Camera& GetCameraData() { return this->camera_; }

#pragma endregion 


private:

	// 回転処理
	void CalcOrientation();

	// フォロー処理
	void FollowFunc();

	// ImGuiの描画
	void DrawImGui();


private:

	// プレイヤー
	Player* player_ = nullptr;

	// モデル
	uint32_t modelHandle_ = 0;
	std::unique_ptr<Model> model_;
	// トランスフォーム
	WorldTransform transform_{};
	// マテリアル
	Material mtl_{};

	// カメラ本体
	Camera camera_{};

	// 初期値
	Vector3 initRotate_{};
	Vector3 initTranslate_{};

	// 回転速度
	float addOrientationSpeed_ = 0.3f;
	// 姿勢計算の補間速度
	float orientationLerpSpeed_ = 0.5f;
	// 相対位置
	Vector3 constOffset_{};
	// プレイヤーからのオフセット
	Vector3 offset_{};


#pragma region System システム

	// LStickの入力
	Vector2 iRStick_{};

	// デッドゾーン
	const float DZone_ = 0.2f;

#pragma endregion 

};


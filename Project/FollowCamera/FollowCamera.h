#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "Input.h"
#include "VectorCalculation.h"
#include "Matrix4x4Calculation.h"
#include "Camera.h"
#include "../Player/Timer/pTimer.h"

#include <random>


// 前方宣言
class Player;
struct Camera;
struct DirectionalLight;
class TInput;


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
	void FuncKeyFunc();

	// シェイクの処理
	void CallShake();


#pragma region Accessor アクセッサ

	// Player
	void SetPlayer(Player* player) { this->player_ = player; }

	// Camera
	Camera& GetCameraData() { return this->camera_; }

	// ForwardVec
	Vector3& GetForwardVec() { return this->forwardVec_; }

	// RightVec
	Vector3& GetRightVec() { return this->rightVec_; }

#pragma endregion 


private:

	// 回転処理
	void CalcStickOrientation();
	void CalcKeyOrientation();

	// フォロー処理
	void FollowFunc();

	// 前方ベクトルを求める
	void CalcForwardVec();

	// 右方ベクトルを求める
	void CalcRightVec();

	// シェイクのエンター処理
	void EnterShakeFunc();

	// シェイク処理
	void ShakeFunc();

	// シェイクの終了処理
	void ExsitShakeFunc();

	// レベルに応じてシェイクの強さの計算
	float CalcShakeIntensityForLevel(int level) const;

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

	// 前方ベクトル
	Vector3 forwardVec_{};
	// 右方ベクトル
	Vector3 rightVec_{};

	// シェイクしているか
	bool isShake_ = false;

	// シェイクの基本の強さ
	float baseShakeIntensity_ = 0.2f;
	// シェイクの成長率
	float shakeGrowthRate_ = 1.2f;
	// シェイクの強さ
	float shakeIntensity_ = 0.0f;
	// シェイクの時間
	pTimer shakeTimer_{};
	// シェイク開始時のカメラの座標
	Vector3 shakeStartPos_{};


#pragma region System システム

	// LStickの入力
	Vector2 iRStick_{};

	// Keyの入力
	Vector2 iKeys_{};

	// 入力
	TInput* tInput_ = nullptr;

	// デッドゾーン
	const float DZone_ = 0.2f;

	// 乱数生成器
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution;

#pragma endregion 

};


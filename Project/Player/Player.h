#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "Input.h"
#include "VectorCalculation.h"
#include "Matrix4x4Calculation.h"
#include "Func/PlayerFunc.h"


// 前方宣言
struct Camera;
struct DirectionalLight;


/* Playerクラス */
class Player {

public:

	// コンストラクタ、デストラクタ
	Player() = default;
	~Player() = default;

	// コピーコンストラクタ
	Player(uint32_t modelHandle);

	// 初期化、更新、描画
	void Init();
	void Update();
	void Draw3D(Camera& camera, DirectionalLight& light);

	// Aボタンが押された時の処理
	void FuncAButton();

	// stick入力時の処理
	void FuncStickFunc(XINPUT_STATE joyState);


#pragma region Accessor アクセッサ

	// ワールド座標の取得
	Vector3 GetWorldPos() const {
		return { transform_.worldMatrix_.m[3][0], transform_.worldMatrix_.m[3][1], transform_.worldMatrix_.m[3][2] };
	}
	
	// Groundの四隅
	void SetGroundCorners(Vector3 LB, Vector3 RB, Vector3 LF, Vector3 RF) {
		groundCorners_.push_back(LB);
		groundCorners_.push_back(RB);
		groundCorners_.push_back(LF);
		groundCorners_.push_back(RF);
	}

#pragma endregion 


private:

	// 移動処理
	void Move();

	// 移動限界処理
	void MoveLimited();

	// Y軸の姿勢を傾ける処理
	void BodyOrientation();

	// ジャンプのエンター処理
	void EnterJampFunc();

	// ジャンプ処理
	void JumpFunc();

	// ジャンプ終了処理
	void JumpExsit();

	// ストンプのエンター処理
	void EnterStompFunc();

	// ストンプ処理
	void StompFunc();

	// ストンプ終了処理
	void StompExsit();

	// Imguiの描画
	void DrawImGui();


private:

	// モデル
	uint32_t modelHandle_ = 0;
	std::unique_ptr<Model> model_;
	// トランスフォーム
	WorldTransform transform_{};
	// マテリアル
	Material mtl_{};

	// 移動量
	Vector3 velocity_{};
	// 移動速度
	float moveSpeed_ = 0.3f;

	// 姿勢
	float bodyOrientation_ = 0.0f;
	// 姿勢計算の補間速度
	float orientationLerpSpeed_ = 0.1f;
	
	// ジャンプのフラグ
	bool isJumping_ = false;
	// 地面にいるかどうかのフラグ
	bool isGrounded_ = true;
	// Y軸方向の速度
	float jumpVel_ = 0.0f;
	// ジャンプ時の初速
	float jumpForce_ = 25.0f;
	// 重力の強さ
	float jumpGravity_ = 30.0f;
	// ジャンプのフレーム時間ごとの時間経過
	float jumpDeltaTime_ = 0.036f;

	// ストンプのフラグ
	bool isStomping_ = false;
	// Y軸方向の速度
	float stompVel_ = 0.0f;
	// 急降下のスピード
	float stompSpeed_ = 1.0f;
	// 重力の強さ
	float stompGravoty_ = 1.0f;
	// ストンプのフレーム時間ごとの時間経過
	float stompDeltaTime_ = 0.9f;

	// Groundの四隅座標
	std::vector<Vector3> groundCorners_{};
	

#pragma region System システム

	// LStickの入力
	Vector2 iLStick_{};

	// デッドゾーン
	const float DZone_ = 0.2f;

#pragma endregion 

};


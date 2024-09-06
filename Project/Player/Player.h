#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "Input.h"
#include "VectorCalculation.h"
#include "Matrix4x4Calculation.h"


// 前方宣言
class Input;
class AdjustmentItems;
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

	// 移動処理
	void Move(XINPUT_STATE joyState);


#pragma region Accessor アクセッサ

	// ワールド座標の取得
	Vector3 GetWorldPos() const {
		return { transform_.worldMatrix_.m[3][0], transform_.worldMatrix_.m[3][1], transform_.worldMatrix_.m[3][2] };
	}

#pragma endregion 


private:

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
	float stompSpeed_ = 30.0f;
	// 重力の強さ
	float stompGravoty_ = 40.0f;
	


#pragma region System システム


#pragma endregion 

};


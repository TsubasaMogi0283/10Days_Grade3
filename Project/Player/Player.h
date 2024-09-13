#pragma once

#include <list>

#include "Model.h"
#include "WorldTransform.h"
#include "Material.h"
#include "Input.h"
#include "VectorCalculation.h"
#include "Matrix4x4Calculation.h"
#include "Func/PlayerFunc.h"

#include "Parts/IPlayerParts.h"
#include "Parts/Head/PlayerHead.h"
#include "Parts/Torso/PlayerTorso.h"

#include "Effects/CrackEffect/CrackEffect.h"

#include "PlayerAttack.h"
#include "Collider/Collider.h"
#include "Record/Record.h"
#include "Audio.h"

// 前方宣言
struct Camera;
struct DirectionalLight;
class FollowCamera;
class TInput;

// プレイヤー関連のモデルハンドル
struct PlayerAssetsHandle {
	uint32_t head;
	uint32_t torso;
	uint32_t crack;
};


/* Playerクラス */
class Player :public Collider{

public:

	// コンストラクタ、デストラクタ
	Player() = default;
	~Player() = default;

	// コピーコンストラクタ
	Player(PlayerAssetsHandle handles);

	// 初期化、更新、描画
	void Init();
	void Update();
	void Draw3D(Camera& camera, DirectionalLight& light);

	// Aボタンが押された時の処理
	void FuncAButton();

	// stick入力時の処理
	void FuncStickFunc(XINPUT_STATE joyState);

	// key入力操作時の処理
	void FuncKeyFunc();

	/// <summary>
	/// 衝突
	/// </summary>
	void OnCollision()override;

#pragma region Accessor アクセッサ

	// ワールド座標の取得
	Vector3 GetWorldPosition() override {
		return { transform_.worldMatrix_.m[3][0], transform_.worldMatrix_.m[3][1], transform_.worldMatrix_.m[3][2] };
	}

	// Groundの四隅
	void SetGroundCorners(Vector3 LB, Vector3 RB, Vector3 LF, Vector3 RF) {
		groundCorners_.push_back(LB);
		groundCorners_.push_back(RB);
		groundCorners_.push_back(LF);
		groundCorners_.push_back(RF);
	}

	// フォローカメラ
	void SetFollowCamera(FollowCamera* camera) { this->followCamera_ = camera; }

	// 着地状態
	bool IsGrounded() const { return this->isGrounded_; }

	// ジャンプ中か
	bool IsJumping() const { return this->isJumping_; }

	// ストンプ中か
	bool IsStomping() const { return this->isStomping_; }

	// キルストリーク中か
	bool IsKillStreak() const { return this->isKillStreak_; }

	// キルストリークカウント
	int GetKillStreakCount() const { return this->killStrealCount_; }

	/// <summary>
	/// 攻撃の当たり判定を取得
	/// </summary>
	/// <returns></returns>
	PlayerAttack* GetPlayerAttack() const{
		return attack_.get();
	}

	/// <summary>
	/// 落下中かどうか
	/// </summary>
	/// <returns></returns>
	bool GetIsDrop()const {
		return isDrop_;
	}

#pragma endregion 


private:

	// 移動方向を求める
	void CalcStickMoveDirection();
	void CalcKeyMoveDirection();

	// 移動処理
	void StickMove();
	void KeyMove();

	// 移動限界処理
	void MoveLimited();

	// Y軸の姿勢を傾ける処理
	void StickBodyOrientation();
	void KeyBodyOrientation();

	// ジャンプのエンター処理
	void EnterJumpFunc();

	// ジャンプ処理
	void JumpFunc();

	// ジャンプ終了処理
	void ExsitJumpFunc();

	// レベルに応じたジャンプの高さの計算
	float CalcJumpForceForLevel(int level) const;

	// ストンプのエンター処理
	void EnterStompFunc();

	// ストンプの更新処理 
	void StompUpdate();

	// ストンプ処理
	void StompFunc();

	// ストンプ硬直処理　
	void StompStunFunc();

	// ストンプ終了処理
	void ExsitStompFunc();

	// 亀裂インスタンスの作成&配列追加
	void AddNewCrack();

	// レベルに応じた亀裂のスケールの計算
	float CalcCrackScaleForLevel(int level) const;

	// Imguiの描画
	void DrawImGui();

	/// <summary>
	/// スピード管理
	/// </summary>
	void SpeedManagiment();

	/// <summary>
	/// 点滅
	/// </summary>
	void Flashing();

private:

	// プレイヤー関連のモデルハンドル
	PlayerAssetsHandle handles_{};

	// トランスフォーム
	WorldTransform transform_{};
	// マテリアル
	Material mtl_{};

	// 移動方向
	Vector3 stickMoveDirection_{};
	Vector3 keyMoveDirection_{};
	// 移動量
	Vector3 velocity_{};
	// 移動速度
	float moveSpeed_ = 0.3f;

	// 姿勢
	float bodyOrientation_ = 0.0f;
	// 姿勢計算の補間速度
	float orientationLerpSpeed_ = 0.1f;

	// 地面のY地点
	float groundLevel_ = 0.0;
	
	// ジャンプのフラグ
	bool isJumping_ = false;
	// 地面にいるかどうかのフラグ
	bool isGrounded_ = true;
	// Y軸方向の速度
	float jumpVel_ = 0.0f;
	// ベースのジャンプの初速速度
	float baseJumpForce_ = 20.0f;
	// ジャンプの成長率
	float jumpGrowthRate_ = 1.1f;
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
	// ストンプの開始Ｙ座標
	float stompStartYPos_ = 0.0f;
	// ストンプのタイマー
	pTimer stompTimer_{};
	// ストンプ硬直タイマー
	pTimer stompStunTime_{};

	// Groundの四隅座標
	std::vector<Vector3> groundCorners_{};

	//落下しているか
	bool isDrop_ = false;

	//移動スピードが減るかどうか
	bool isSpeedDown_ = false;
	//スピード倍率
	float speedMagnification_ = 1.0f;
	//減速中の時間
	int speedDownTime_ = 0;

	// キルが確認されたかのフラグ
	bool isKillConfirmed_ = false;

	// キルストリーク
	bool isKillStreak_ = false;
	int killStrealCount_ = 2;
	
	//当たり判定
	std::unique_ptr<PlayerAttack>attack_ = nullptr;


#pragma region System システム

	// LStickの入力
	Vector2 iLStick_{};

	// Keyの入力
	Vector2 iKeys_{};

	// 入力
	TInput* tInput_ = nullptr;

	// デッドゾーン
	const float DZone_ = 0.2f;

	// 乱数生成器
	std::mt19937 randomEngine_;

#pragma endregion 


private: // 体のパーツ

	// 体のパーツ配列
	std::vector<std::shared_ptr<IPlayerParts>> iParts_;
	

private: // エフェクト

	// 亀裂エフェクト配列
	std::list<std::shared_ptr<CrackEffect>> cracks_;

	// 亀裂基本スケール
	float baseCrackScale_ = 5.0f;
	// 成長率
	float crackScaleGrowthScale_ = 1.2f;


private: // フォローカメラ

	FollowCamera* followCamera_ = nullptr;


private:

	Record* record_ = nullptr;


	Audio* jumpSE_ = nullptr;
	uint32_t jumpSEHandle_ = 0u;
	int32_t jumpTime_ = 0;

};


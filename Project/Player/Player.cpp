#include "Player.h"
#include "Input.h"
#include "FollowCamera/FollowCamera.h"
#include <algorithm>


// コピーコンストラクタ
Player::Player(PlayerAssetsHandle handles)
{
	this->handles_ = handles;
}


// 初期化処理
void Player::Init()
{
	// モデルの初期化
	model_.reset(Model::Create(handles_.player));

	// トランスフォームの初期化
	transform_.Initialize();
	transform_.translate_.y = 1.0f;

	// マテリアルの初期化
	mtl_.Initialize();
}


// 更新処理
void Player::Update()
{
	// トランスフォームの更新
	transform_.Update();

	// マテリアルの更新
	mtl_.Update();

	// 移動方向を求める
	CalcMoveDirection();

	// ジャンプ処理
	if (isJumping_) { // フラグが立っていたら入る
		JumpFunc();
	}

	// ストンプの処理
	if (isStomping_) { // フラグが立っていたら入る
		StompFunc();
	}


#ifdef _DEBUG
	// ImGuiの描画
	DrawImGui();
#endif // _DEBUG
}


// 描画処理
void Player::Draw3D(Camera& camera, DirectionalLight& light)
{
	model_->Draw(transform_, camera, mtl_, light);
}


// Aボタンが押された時の処理
void Player::FuncAButton()
{
	// ジャンプしていなければジャンプ
	if (!isJumping_) {
		EnterJampFunc();
		return;
	}

	// ジャンプしていたらストンプ処理
	if (isJumping_ && !isStomping_) {
		EnterStompFunc();
		return;
	}
}


// stick入力時の処理
void Player::FuncStickFunc(XINPUT_STATE joyState)
{
	//コントローラーの入力
	iLStick_ = {
		.x = (static_cast<float>(joyState.Gamepad.sThumbLX) / SHRT_MAX * 1.0f),
		.y = (static_cast<float>(joyState.Gamepad.sThumbLY) / SHRT_MAX * 1.0f),
	};

	// 移動処理
	Move();

	// Y軸の姿勢処理
	BodyOrientation();
}


// 移動方向を求める
void Player::CalcMoveDirection()
{
	// カメラの前方と右方
	Vector3 forward = followCamera_->GetForwardVec();
	Vector3 right = followCamera_->GetRightVec();

	moveDirection_ = {
		.x = (iLStick_.x * right.x) + (iLStick_.y * forward.x),
		.y = 0.0f,
		.z = (iLStick_.x * right.z) + (iLStick_.y * forward.z),
	};
}


// 移動処理
void Player::Move()
{
	// ストンプ中は移動できない。早期return
	if (isStomping_) { return; }

	// velocityは0で更新
	velocity_ = { 0.0f, 0.0f, 0.0f };

	// 移動量の計算
	if (std::abs(iLStick_.x) > DZone_ || std::abs(iLStick_.y) > DZone_) {

		// 移動量の計算(カメラの前方と右方に基づく)
		velocity_ = moveDirection_;

		// 移動量を正規化し速さを乗算
		velocity_ = VectorCalculation::Multiply(VectorCalculation::Normalize(velocity_), moveSpeed_);

		// 移動
		transform_.translate_ = VectorCalculation::Add(transform_.translate_, velocity_);

		// 移動限界処理
		MoveLimited();
	}
}


// 移動限界処理
void Player::MoveLimited()
{
	float minX{}, maxX{}, minZ{}, maxZ{};

	// 四隅の頂点から最大・最小値を計算
	for (const auto& corner : groundCorners_) {
		minX = min(groundCorners_[0].x, corner.x);
		maxX = max(groundCorners_[0].x, corner.x);
		minZ = min(groundCorners_[0].z, corner.z);
		maxZ = max(groundCorners_[0].z, corner.z);
	}
	
	// 移動限界を設定
	transform_.translate_.x = max(transform_.translate_.x, minX);
	transform_.translate_.x = min(transform_.translate_.x, maxX);
	transform_.translate_.z = max(transform_.translate_.z, minZ);
	transform_.translate_.z = min(transform_.translate_.z, maxZ);
}


// Y軸の姿勢を傾ける処理
void Player::BodyOrientation()
{
	if (std::abs(iLStick_.x) > DZone_ || std::abs(iLStick_.y) > DZone_) {

		// 正規化した移動方向
		Vector3 normalizeDirection = VectorCalculation::Normalize(moveDirection_);

		// 目標回転角度
		float targetAngle = std::atan2(normalizeDirection.x, normalizeDirection.z);

		// 現在の角度と目標角度から最短を求める
		float shortestAngle = pFunc::ShortestAngle(transform_.rotate_.y, targetAngle);

		// 現在の角度を目標角度の間を補間
		transform_.rotate_.y = pFunc::Lerp(transform_.rotate_.y, transform_.rotate_.y + shortestAngle, orientationLerpSpeed_);
	}
}


// ボタン押下時に呼び出されるジャンプのエンター処理
void Player::EnterJampFunc()
{
	isJumping_ = true; // ジャンプ中
	isGrounded_ = false; // 地面から離れた状態
	jumpVel_ = CalcJumpForceForLevel(killStrealCount_); // ジャンプの初速を求める
}


// ジャンプ処理
void Player::JumpFunc()
{
	// 重力をY軸速度に加える
	jumpVel_ -= jumpGravity_ * jumpDeltaTime_;

	// プレイヤーのY軸方向の移動を更新
	transform_.translate_.y += jumpVel_ * jumpDeltaTime_;

	// 地面との接触判定
	if (transform_.translate_.y <= 0.0f) {

		// 地面に戻す
		transform_.translate_.y = 1.0f;
		// 着地状態
		isGrounded_ = true;
		// 終了処理
		JumpExsit();
	}
}


// ジャンプ終了処理
void Player::JumpExsit()
{
	isJumping_ = false; // ジャンプ終了
	jumpVel_ = 0.0f; // Y軸速度をリセット
}


// レベルに応じたジャンプの高さの計算
float Player::CalcJumpForceForLevel(int level) const
{
	return baseJumpForce_ * float(std::pow(jumpGrowthRate_, level));
}


// ストンプのエンター処理
void Player::EnterStompFunc()
{
	JumpExsit(); // ジャンプの終了処理
	isStomping_ = true; // ストンプ中
	stompVel_ = -stompSpeed_; // ストンプの速度を設定
}


// ストンプ処理
void Player::StompFunc()
{
	// 重力をY軸速度に加える
	stompVel_ -= stompGravoty_ * stompDeltaTime_;

	// 最大落下速度を制限する
	const float maxFallSpeed = -30.0f;  // 例: 最大速度
	if (stompVel_ < maxFallSpeed) {
		stompVel_ = maxFallSpeed;
	}

	// プレイヤーのY軸方向の移動を更新
	transform_.translate_.y += stompVel_ * stompDeltaTime_;

	// 地面との接触判定
	if (transform_.translate_.y <= 0.0f) {

		// 地面に戻す
		transform_.translate_.y = 1.0f;
		// 着地状態
		isGrounded_ = true;
		// ストンプ終了処理
		StompExsit();
	}
}


// ストンプ終了処理
void Player::StompExsit()
{
	isStomping_ = false; // ストンプ終了
	stompVel_ = 0.0f; // Y軸速度をリセット
}


// Imguiの描画
void Player::DrawImGui()
{
	if (ImGui::TreeNode("Player")) {

		ImGui::Text("トランスフォーム");
		ImGui::DragFloat3("Scale", &transform_.scale_.x, 0.01f, 0.1f, 10.0f);
		ImGui::DragFloat3("Rotate", &transform_.rotate_.x, 0.001f);
		ImGui::DragFloat3("Transform", &transform_.translate_.x, 0.01f);
		ImGui::Text("");

		ImGui::Text("キル関連数値");
		ImGui::Checkbox("キルストリーク", &isKillStreak_);
		ImGui::SliderInt("キルストリーク数", &killStrealCount_, 0, 5);
		ImGui::Text("");

		ImGui::Text("姿勢関連数値");
		ImGui::DragFloat("姿勢の補間速度", &orientationLerpSpeed_, 0.01f);
		ImGui::Text("");

		ImGui::Text("入力関連数値");
		ImGui::DragFloat2("L_Stick", &iLStick_.x, 0.0f);
		float atan = std::atan2(iLStick_.x, iLStick_.y);
		ImGui::DragFloat("Stick_tan2", &atan, 0.0f);
		ImGui::Text("");

		ImGui::Text("ジャンプ関連数値");
		ImGui::DragFloat("j初速", &baseJumpForce_, 0.01f);
		ImGui::DragFloat("j重力", &jumpGravity_, 0.01f);
		ImGui::DragFloat("jデルタタイム", &jumpDeltaTime_, 0.001f);
		ImGui::Checkbox("Is_Jump", &isJumping_);
		ImGui::Checkbox("Is_Grounded", &isGrounded_);
		ImGui::DragFloat("j速度", &jumpVel_, 0.0f);
		ImGui::Text("");

		ImGui::Text("ストンプ関連数値");
		ImGui::DragFloat("s降下速度", &stompSpeed_, 0.01f);
		ImGui::DragFloat("s重力", &stompGravoty_, 0.01f);
		ImGui::DragFloat("sデルタタイム", &stompDeltaTime_, 0.001f);
		ImGui::Checkbox("Is_Stomp", &isStomping_);
		ImGui::DragFloat("s速度", &stompVel_, 0.0f);
		ImGui::Text("");

		ImGui::TreePop();
	}
}


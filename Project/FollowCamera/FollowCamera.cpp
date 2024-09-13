#include "FollowCamera.h"
#include "Player/Player.h"


// コピーコンストラクタ
FollowCamera::FollowCamera(uint32_t modelHandle)
{
	this->modelHandle_ = modelHandle;
}


// 初期化処理
void FollowCamera::Init()
{
	// モデルの初期化
	model_.reset(Model::Create(modelHandle_));

	// 初期値の設定
	initRotate_ = { 0.24f,0.0f,0.0f };
	initTranslate_ = { 0.0f, 0.2f,0.0f };

	// カメラの初期化
	camera_.Initialize();
	camera_.rotate_ = initRotate_;
	camera_.translate_ = initTranslate_;
	camera_.Update();

	// トランスフォームの初期化
	transform_.Initialize();

	// マテリアルの初期化
	mtl_.Initialize();

	// 相対位置
	constOffset_ = { 0.0f, 20.0f, -50.0f };

	// オフセットの設定
	offset_ = constOffset_;

}


// 更新処理
void FollowCamera::Update()
{
	// カメラの更新
	camera_.Update();

	// トランスフォームの更新
	transform_.Update();
	transform_.rotate_ = camera_.rotate_;
	transform_.translate_ = camera_.translate_;

	// マテリアルの更新
	mtl_.Update();

	// 前方ベクトルを求める
	CalcForwardVec();

	// 右方ベクトルを求める
	CalcRightVec();


#ifdef _DEBUG
	// ImGuiの描画
	DrawImGui();
#endif // _DEBUG
}


// 描画処理
void FollowCamera::Draw3D(Camera& camera, DirectionalLight& light)
{
	model_->Draw(transform_, camera, mtl_, light);
}


// stick入力時の処理
void FollowCamera::FuncStickFunc(XINPUT_STATE joyState)
{
	//コントローラーの入力
	iRStick_ = {
		.x = (static_cast<float>(joyState.Gamepad.sThumbRX) / SHRT_MAX * 1.0f),
		.y = (static_cast<float>(joyState.Gamepad.sThumbRY) / SHRT_MAX * 1.0f),
	};

	// 回転処理
	CalcOrientation();

	// フォロー処理
	FollowFunc();

	// シェイク処理
	if (isShake_) { // フラグが立っていたら入る
		ShakeFunc();
	}
}


// シェイクの処理
void FollowCamera::CallShake()
{
	EnterShakeFunc();
}


// 回転処理
void FollowCamera::CalcOrientation()
{
	if (std::abs(iRStick_.x) > DZone_ || std::abs(iRStick_.y) > DZone_) {

		// 目標回転角度
		float addAngle = iRStick_.x * addOrientationSpeed_;

		// 現在の角度と目標角度の計算
		float targetAngle = camera_.rotate_.y + addAngle;

		// 現在の角度を目標角度の間を補間
		camera_.rotate_.y = pFunc::Lerp(camera_.rotate_.y, targetAngle, orientationLerpSpeed_);
	}
}


// フォロー処理
void FollowCamera::FollowFunc()
{
	// オフセットの設定
	offset_ = constOffset_;

	// カメラの前方方向に基づいてカメラのオフセットを回転させる
	Matrix4x4 rotateMat = Matrix4x4Calculation::MakeIdentity4x4();
	rotateMat = Matrix4x4Calculation::MakeRotateYMatrix(camera_.rotate_.y);

	// カメラのオフセットの回転
	offset_ = VectorCalculation::TransformCalculation(offset_, rotateMat);

	// プレイヤーの位置にオフセットを加えてカメラの位置を更新
	camera_.translate_ = VectorCalculation::Add(player_->GetWorldPosition(), offset_);

	// Yの位置だけ固定
	camera_.translate_.y = constOffset_.y;
}


// 前方ベクトルを求める
void FollowCamera::CalcForwardVec()
{
	// デフォルトの前方ベクトル
	Vector3 defForward = { 0.0f, 0.0f, 1.0f };

	// Y軸の回転行列
	Matrix4x4 rotateYMat = 
		Matrix4x4Calculation::MakeRotateYMatrix(camera_.rotate_.y);

	// 前方ベクトルを求める
	forwardVec_ = VectorCalculation::TransformCalculation(defForward, rotateYMat);
}


// 右方ベクトルを求める
void FollowCamera::CalcRightVec()
{
	// デフォルトの前方ベクトル
	Vector3 defForward = { 1.0f, 0.0f, 0.0f };

	// Y軸の回転行列
	Matrix4x4 rotateYMat =
		Matrix4x4Calculation::MakeRotateYMatrix(camera_.rotate_.y);

	// 前方ベクトルを求める
	rightVec_ = VectorCalculation::TransformCalculation(defForward, rotateYMat);
}


// シェイクのエンター処理
void FollowCamera::EnterShakeFunc()
{
	// シェイク開始時の座標を保存しておく
	shakeStartPos_ = camera_.translate_;

	isShake_ = true; // フラグを立てる
	shakeIntensity_ = // 強さを求める
		CalcShakeIntensityForLevel(player_->GetKillStreakCount());
	shakeTimer_.Init(0.0f, 30.0f); // 時間の設定(0.5秒)
	shakeTimer_.Start(); // タイマースタート
	distribution =  // ランダム生成の範囲設定
		std::uniform_real_distribution<float>(-1.0f, 1.0f); 
}


// シェイク処理
void FollowCamera::ShakeFunc()
{
	// 時間の更新
	shakeTimer_.Update();

	// タイマーーが終了していれば終了
	if (shakeTimer_.IsFinish()) {
		ExsitShakeFunc();
		return;
	}

	// 残り時間に応じでシェイクの強度を減衰させる(線形に減衰)
	float currentIntensity = shakeIntensity_ * (1.0f - shakeTimer_.GetRatio());

	// ランダムに変動させる処理
	Vector3 shakeOffset = {
		distribution(generator) * currentIntensity,
		distribution(generator) * currentIntensity,
		distribution(generator) * currentIntensity,
	};

	// カメラの座標に加算
	camera_.translate_ = VectorCalculation::Add(camera_.translate_, shakeOffset);
}


// シェイクの終了処理
void FollowCamera::ExsitShakeFunc()
{
	isShake_ = false; // シェイク終了
	shakeIntensity_ = 0.0f; // 強さを0で初期化
	shakeTimer_.Clear(); // タイマーをクリア
}


// レベルに応じてシェイクの強さの計算
float FollowCamera::CalcShakeIntensityForLevel(int level) const
{
	return baseShakeIntensity_ * float(std::pow(shakeGrowthRate_, level));
}


// ImGuiの描画
void FollowCamera::DrawImGui()
{
	if (ImGui::TreeNode("FollowCamera")) {

		ImGui::Text("トランスフォーム");
		ImGui::DragFloat3("Rotate", &camera_.rotate_.x, 0.001f);
		ImGui::DragFloat3("Transform", &camera_.translate_.x, 0.01f);
		ImGui::Text("");

		ImGui::Text("シェイク関連数値");
		ImGui::Checkbox("IsShake", &isShake_);
		ImGui::DragFloat("シェイクの強さ", &shakeIntensity_, 0.0f);
		if (ImGui::Button("EnterShakeFunc")) {
			EnterShakeFunc();
		}
		ImGui::Text("");

		ImGui::Text("ベクトル");
		ImGui::DragFloat3("Forward", &forwardVec_.x, 0.0f);
		ImGui::DragFloat3("RightVec", &rightVec_.x, 0.0f);

		ImGui::Text("入力関連数値");
		ImGui::DragFloat2("R_Stick", &iRStick_.x, 0.0f);
		ImGui::Text("");
		
		ImGui::TreePop();
	}
}


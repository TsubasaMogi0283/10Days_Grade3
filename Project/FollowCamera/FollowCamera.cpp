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
	camera_.translate_ = VectorCalculation::Add(player_->GetWorldPos(), offset_);

	// Yの位置だけ固定
	camera_.translate_.y = constOffset_.y;
}


// ImGuiの描画
void FollowCamera::DrawImGui()
{
	if (ImGui::TreeNode("FollowCamera")) {

		ImGui::Text("トランスフォーム");
		ImGui::DragFloat3("Rotate", &camera_.rotate_.x, 0.001f);
		ImGui::DragFloat3("Transform", &camera_.translate_.x, 0.01f);
		ImGui::Text("");

		ImGui::Text("入力関連数値");
		ImGui::DragFloat2("R_Stick", &iRStick_.x, 0.0f);
		ImGui::Text("");
		
		ImGui::TreePop();
	}
}

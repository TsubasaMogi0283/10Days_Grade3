#include "Player.h"
#include "Input.h"
#include "AdjustmentItems.h"


// コピーコンストラクタ
Player::Player(uint32_t modelHandle)
{
	// モデルハンドの設定
	this->modelHandle_ = modelHandle;
}


// 初期化処理
void Player::Init()
{
	// モデルの初期化
	model_.reset(Model::Create(modelHandle_));

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


// 移動処理
void Player::Move(XINPUT_STATE joyState)
{
	// velocityは0で更新
	velocity_ = { 0.0f, 0.0f, 0.0f };

	//コントローラーの入力
	Vector2 leftStickInput = {
		.x = (static_cast<float>(joyState.Gamepad.sThumbLX) / SHRT_MAX * 1.0f),
		.y = (static_cast<float>(joyState.Gamepad.sThumbLY) / SHRT_MAX * 1.0f),
	};

	// デッドゾーン
	const float DZone = 0.2f;

	// 移動量の計算
	if (std::abs(leftStickInput.x) > DZone || std::abs(leftStickInput.y) > DZone) {

		// 移動量
		velocity_ = {
			.x = leftStickInput.x,
			.z = leftStickInput.y,
		};

		// 移動量を正規化し速さを乗算
		velocity_ = VectorCalculation::Multiply(VectorCalculation::Normalize(velocity_), moveSpeed_);

		// 移動
		transform_.translate_ = VectorCalculation::Add(transform_.translate_, velocity_);

		// 移動限界
		const float kMoveMit = 100.0f;
		transform_.translate_.x = max(transform_.translate_.x, -kMoveMit);
		transform_.translate_.x = min(transform_.translate_.x, +kMoveMit);
		transform_.translate_.z = max(transform_.translate_.z, -kMoveMit);
		transform_.translate_.z = min(transform_.translate_.z, +kMoveMit);
	}
}


// ジャンプ処理
void Player::JumpFunc()
{
	// フラグが折れていたらフラグを建て、数値の設定をする
	if (!isJump_) {
		isJump_ = true;
	}

}


// Imguiの描画
void Player::DrawImGui()
{
	if (ImGui::TreeNode("Player")) {

		ImGui::Text("Transform"); 
		ImGui::DragFloat3("Scale", &transform_.scale_.x, 0.01f, 0.1f, 10.0f);
		ImGui::DragFloat3("Rotate", &transform_.rotate_.x, 0.001f);
		ImGui::DragFloat3("Transform", &transform_.translate_.x, 0.01f);
		ImGui::Text("");

		ImGui::Text("Jump_Function");
		ImGui::Checkbox("Is_Jump", &isJump_);

		ImGui::TreePop();
	}
}


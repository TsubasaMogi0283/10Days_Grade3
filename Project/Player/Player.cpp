#include "Player.h"
#include "Input.h"
#include "AdjustmentItems.h"


// コピーコンストラクタ
Player::Player(uint32_t modelHandle)
{
	// モデルハンドの設定
	this->modelHandle_ = modelHandle;

	// アジャストメントマネージャー
	this->itemManager_ = AdjustmentItems::GetInstance();
}


// 初期化処理
void Player::Init()
{
	// モデルの初期化
	model_.reset(Model::Create(modelHandle_));

	// トランスフォームの初期化
	transform_.Initialize();

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


// Imguiの描画
void Player::DrawImGui()
{
	if (ImGui::TreeNode("Player")) {

		ImGui::Text("Transform"); 
		ImGui::DragFloat3("Scale", &transform_.scale_.x, 0.01f, 0.1f, 10.0f);
		ImGui::DragFloat3("Rotate", &transform_.rotate_.x, 0.001f);
		ImGui::DragFloat3("Transform", &transform_.translate_.x, 0.01f);

		ImGui::TreePop();
	}
}


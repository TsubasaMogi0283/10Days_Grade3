#include "StompSpeed.h"
#include <random>


// コピーコンストラクタ
pEffect::StompSpeed::StompSpeed(uint32_t handle)
{
	this->modelHandle_ = handle;
}


// 初期化処理
void pEffect::StompSpeed::Init()
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
void pEffect::StompSpeed::Update()
{
	// トランスフォームの更新
	transform_.Update();

	// マテリアルの更新
	mtl_.Update();

	// アクティブなら
	if (isActive_)
		// 乱数回転処理
		RandOrientation();


#ifdef _DEBUG
	// ImGuiの描画
	DrawImGui();
#endif // _DEBUG
}


// 描画処理
void pEffect::StompSpeed::Draw3D(Camera& camera, DirectionalLight& light)
{
	if (!isActive_) { return; }
	model_->Draw(transform_, camera, mtl_, light);
}


// 乱数回転処理
void pEffect::StompSpeed::RandOrientation()
{
	// ランダムな回転値を生成するための乱数エンジン
	std::random_device rd;
	std::mt19937 mt(rd());

	// 回転値の範囲（-180度から180度まで）
	std::uniform_real_distribution<float> dist(-4.0f, +40.0f);

	float setRad = dist(mt);

	transform_.rotate_.y = setRad;
}


// Imguiの描画
void pEffect::StompSpeed::DrawImGui()
{
	if (ImGui::TreeNode("StompSpeed")) {

		ImGui::Checkbox("アクティブ", &isActive_);
		ImGui::Text("");

		ImGui::Text("トランスフォーム");
		ImGui::DragFloat3("Scale", &transform_.scale_.x, 0.01f, 0.1f, 10.0f);
		ImGui::DragFloat3("Rotate", &transform_.rotate_.x, 0.001f);
		ImGui::DragFloat3("Transform", &transform_.translate_.x, 0.01f);
		ImGui::Text("");

		ImGui::TreePop();
	}
}

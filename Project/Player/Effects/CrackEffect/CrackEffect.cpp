#include "CrackEffect.h"



// コピーコンストラクタ
CrackEffect::CrackEffect(uint32_t handle)
{
	this->modelHandle_ = handle;
}


// 初期化処理
void CrackEffect::Init()
{
	// モデルの初期化
	model_.reset(Model::Create(modelHandle_));

	// 初期値
	float initScale = 8.0f;

	// トランスフォームの初期化
	transform_.Initialize();
	transform_.scale_ = { initScale, initScale, initScale };
	transform_.translate_.y = 0.01f;

	// マテリアルの初期化
	mtl_.Initialize();
}


// 更新処理
void CrackEffect::Update()
{
	// トランスフォームの更新
	transform_.Update();

	// マテリアルの更新
	mtl_.Update();

	// アクティブなら
	if (isActive_) {

	}
 

#ifdef _DEBUG
	// ImGuiの描画
	DrawImGui();
#endif // _DEBUG
}


// 描画処理
void CrackEffect::Draw3D(Camera& camera, DirectionalLight& light)
{
	if (!isActive_) { return; }
	model_->Draw(transform_, camera, mtl_, light);
}


// Imguiの描画
void CrackEffect::DrawImGui()
{
	if (ImGui::TreeNode("CrackEffect")) {

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

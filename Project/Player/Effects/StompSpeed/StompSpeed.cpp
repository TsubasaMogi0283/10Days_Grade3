#include "StompSpeed.h"



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


#ifdef _DEBUG
	// ImGuiの描画
	DrawImGui();
#endif // _DEBUG
}


// 描画処理
void pEffect::StompSpeed::Draw3D(Camera& camera, DirectionalLight& light)
{
	model_->Draw(transform_, camera, mtl_, light);
}


// Imguiの描画
void pEffect::StompSpeed::DrawImGui()
{
	if (ImGui::TreeNode("Player")) {



		ImGui::TreePop();
	}
}

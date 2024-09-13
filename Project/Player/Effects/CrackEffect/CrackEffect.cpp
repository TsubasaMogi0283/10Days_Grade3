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

	// 乱数生成器の作成
	// ランダムな回転値を生成するための乱数エンジン
	std::random_device seedGenerator;
	randomEngine_.seed(seedGenerator());

	// アクティブする
	isActive_ = true;

	// タイマーの設定
	timer_.Init(0.0f, 3.0f * 60.0f);
	timer_.Start();
}


// 更新処理
void CrackEffect::Update()
{
	// トランスフォームの更新
	transform_.Update();

	// マテリアルの更新
	mtl_.Update(); 

	// alpha値を減らす処理
	if (SubAlpha()) {
		isActive_ = false;
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


// alpha値を減らす処理
bool CrackEffect::SubAlpha()
{
	// タイマーの更新
	timer_.Update();

	// イージング処理でalpha値を減らす
	mtl_.color_.w =
		1.0f + (0.0f - 1.0f) * Ease::InCubic(timer_.GetRatio());

	// タイマーが終了したらtrueを返す
	if (timer_.IsFinish()) {
		return true;
	}

	return false;
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


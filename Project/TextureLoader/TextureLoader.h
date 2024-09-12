#pragma once

//読み込みに時間がかかる
//応急処置としてのクラス

class TextureLoader final{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextureLoader() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextureLoader() = default;

public:

	//インスタンス
	static TextureLoader* GetInstance();

	//コピーコンストラクタ禁止
	TextureLoader(const TextureLoader& textureLoader) = delete;

	//代入演算子を無効にする
	TextureLoader& operator=(const TextureLoader& textureLoader) = delete;



public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();




};


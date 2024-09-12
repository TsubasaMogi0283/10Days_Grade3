#pragma once

#include <TransformationMatrix.h>


#include "Matrix4x4Calculation.h"
#include <VertexData.h>


#include <d3dx12.h>



#include "Camera.h"
#include "Transform.h"
#include "Particle.h"
#include <AccelerationField.h>
#include "ModelManager.h"
#include <random>


enum ParticleMoveType {
	
	//通常の放出
	NormalRelease,
	//鉛直投げ上げ
	ThrowUp,
	//自由落下
	FreeFall,
};


struct Material;
struct DirectionalLight;

struct Emitter {
	//エミッタのTransform;
	Transform transform;
	//発生数
	uint32_t count;
	//発生頻度
	float frequency;
	//頻度用時刻
	float frequencyTime;
};

class Particle3D {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Particle3D()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="moveType"></param>
	/// <returns></returns>
	static Particle3D* Create(uint32_t &modelHandle,uint32_t moveType);


private:

#pragma region パーティクルの設定で使う関数

	//パーティクルの初期化をする関数
	Particle MakeNewParticle(std::mt19937& randomEngine);

	//Emitterで発生させる
	std::list<Particle> Emission(const Emitter& emmitter, std::mt19937& randomEngine);

#pragma endregion

public:

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera"></param>
	void Update(Camera& camera);

	/// <summary>
	/// 平行光源
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="material"></param>
	/// <param name="directionalLight"></param>
	void Draw(Camera& camera,Material &material,DirectionalLight& directionalLight);


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Particle3D()=default;




public:



	/// <summary>
	/// 透明になっていくようにするかどうか
	/// </summary>
	/// <param name="isToTransparent"></param>
	void SetIsToTransparent(bool isToTransparent) {
		this->isToTransparent_ = isToTransparent;
	}



#pragma region エミッタの中の設定
	

	#pragma region SRT
	//Scale
	void SetScale(Vector3 scale) {
		this->emitter_.transform.scale = scale;
	}
	
	//Rotate
	void SetRotate(Vector3 rotate) {
		this->emitter_.transform.rotate = rotate;
	}
	Vector3 GetRotate() {
		return emitter_.transform.rotate;
	}

	//Translate
	void SetTranslate(Vector3 translate) {
		this->emitter_.transform.translate = translate;
	}
	Vector3 GetTranslate() {
		return emitter_.transform.translate;
	}

	#pragma endregion

	//発生数
	void SetCount(uint32_t count) {
		this->emitter_.count = count;
	}
	//発生頻度
	void SetFrequency(float frequency){
		this->emitter_.frequency = frequency;
	}
	//発生頻度を設定
	void SetFrequencyTime(float frequencyTime){
		this->emitter_.frequencyTime = frequencyTime;
	}


#pragma endregion


private:


	//頂点リソースを作る
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	std::vector<VertexData> vertices_{};

	//表示する数
	int32_t instanceCount_ = 1;


	//インスタンス
	ComPtr<ID3D12Resource>instancingResource_ = nullptr;

	//最大数
	static const int32_t MAX_INSTANCE_NUMBER_ = 7;
	//描画すべきインスタンス数
	uint32_t numInstance_ = 0;

	int instancingIndex_=0;

	//パーティクル
	std::list<Particle>particles_;
	ParticleForGPU* instancingData_ = nullptr;


	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//動きの種類
	uint32_t moveType_ = ThrowUp;

	//透明になっていくか
	bool isToTransparent_ = true;


	//エミッタの設定
	Emitter emitter_ = {};
	const float DELTA_TIME = 1.0f / 60.0f;


	//鉛直投げ上げ
	float velocityY_ = 1.2f;



	//インスタンス
	ComPtr<ID3D12Resource>cameraResource_ = nullptr;
	Vector3* cameraPositionData_ = {};
	Vector3 cameraPosition_ = {};
};

#include "Particle3D.h" 


#include <cassert>
#include <numbers>

#include <Camera.h>
#include <TextureManager.h>
#include <PipelineManager.h>
#include "DirectXSetup.h"

#include "Material.h"
#include "DirectionalLight.h"

#include <VectorCalculation.h>


#include <Collision.h>
#include "SrvManager.h"
Particle3D* Particle3D::Create(uint32_t& modelHandle, uint32_t moveType) {
	Particle3D* particle3D = new Particle3D();

	//Addでやるべきとのこと
	PipelineManager::GetInstance()->GenerateParticle3DPSO();

#pragma region デフォルトの設定 
	particle3D->emitter_.count = 100;
	//0.5秒ごとに発生
	particle3D->emitter_.frequency = 0.5f;
	//発生頻度用の時刻。0.0で初期化
	particle3D->emitter_.frequencyTime = 0.0f;
	//SRT
	particle3D->emitter_.transform.scale = { 10.0f,10.0f,10.0f };
	particle3D->emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
	particle3D->emitter_.transform.translate = { 0.0f,0.0f,4.0f };

#pragma endregion
	
	//テクスチャの読み込み
	particle3D->textureHandle_ = TextureManager::GetInstance()->LoadTexture(ModelManager::GetInstance()->GetModelData(modelHandle).textureFilePath);

	//動きの種類
	particle3D->moveType_ = moveType;

	//頂点リソースを作る
	particle3D->vertices_ = ModelManager::GetInstance()->GetModelData(modelHandle).vertices;
	particle3D->vertexResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * particle3D->vertices_.size());



	//リソースの先頭のアドレスから使う
	particle3D->vertexBufferView_.BufferLocation = particle3D->vertexResource_->GetGPUVirtualAddress();
	//使用するリソースは頂点のサイズ
	particle3D->vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * particle3D->vertices_.size());
	//１頂点あたりのサイズ
	particle3D->vertexBufferView_.StrideInBytes = sizeof(VertexData);


	//頂点バッファにデータを書き込む
	VertexData* vertexData = nullptr;
	particle3D->vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, particle3D->vertices_.data(), sizeof(VertexData) * particle3D->vertices_.size());
	particle3D->vertexResource_->Unmap(0, nullptr);



	//インスタンシング
	particle3D->instancingResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(ParticleForGPU) * MAX_INSTANCE_NUMBER_);
	
	//SRVを作る
	particle3D->instancingIndex_ = SrvManager::GetInstance()->Allocate();
	SrvManager::GetInstance()->CreateSRVForStructuredBuffer(particle3D->instancingIndex_, particle3D->instancingResource_.Get(), MAX_INSTANCE_NUMBER_, sizeof(ParticleForGPU));
	//書き込み
	particle3D->instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&particle3D->instancingData_));




	return particle3D;

}




//生成関数
Particle Particle3D::MakeNewParticle(std::mt19937& randomEngine) {
	std::uniform_real_distribution<float> distribute(-2.0f, 2.0f);
	Particle particle;
	particle.transform.scale = { 1.0f,1.0f,1.0f };
	particle.transform.rotate = { 0.0f,0.0f,0.0f };
	//ランダムの値
	Vector3 randomTranslate = { distribute(randomEngine),distribute(randomEngine),distribute(randomEngine) };
	particle.transform.translate = VectorCalculation::Add(emitter_.transform.translate,randomTranslate);
	if (moveType_ == ThrowUp) {
		Vector3 offset = { .x = randomTranslate.x,.y = 0.1f,.z = randomTranslate.z };
		particle.transform.translate = VectorCalculation::Add(emitter_.transform.translate, offset);

	}
	
	//速度
	std::uniform_real_distribution<float>distVelocity(-1.0f, 1.0f);
	particle.velocity = {distVelocity(randomEngine),distVelocity(randomEngine),distVelocity(randomEngine)};




	//Color
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	particle.color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine),1.0f };
	



	//時間
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0;



	return particle;

}

//エミッタ
std::list<Particle> Particle3D::Emission(const Emitter& emmitter, std::mt19937& randomEngine){
	std::list<Particle> particles;

	for (uint32_t count = 0; count < emmitter.count; ++count) {
		//emmitterで設定したカウントまで増やしていくよ
		particles.push_back(MakeNewParticle(randomEngine));
	}

	return particles;
}


//更新
void Particle3D::Update(Camera& camera){
	

	//C++でいうsrandみたいなやつ
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	
	///時間経過
	emitter_.frequencyTime += DELTA_TIME;
	//頻度より大きいなら
	if (emitter_.frequency <= emitter_.frequencyTime) {
		//パーティクルを作る
		particles_.splice(particles_.end(), Emission(emitter_, randomEngine));
		//余計に過ぎた時間も加味して頻度計算する
		emitter_.frequencyTime -= emitter_.frequency;
	}


	//座標の計算など
	numInstance_ = 0;
	for (std::list<Particle>::iterator particleIterator = particles_.begin();
		particleIterator != particles_.end();++particleIterator) {
		
		//行列の初期化
		Matrix4x4 worldMatrix = {};
		Matrix4x4 scaleMatrix = {};
		Matrix4x4 translateMatrix = {};
		Matrix4x4 billBoardMatrix = {};
		Matrix4x4 backToFrontMatrix = {};



		switch (moveType_) {
			
		case NormalRelease:
			#pragma region //通常の放出
			//if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			//
			//	continue;
			//}
			//強制的にビルボードにするよ
			
			particleIterator->transform.translate.x += particleIterator->velocity.x * DELTA_TIME;
			particleIterator->transform.translate.y += particleIterator->velocity.y * DELTA_TIME;
			particleIterator->transform.translate.z += particleIterator->velocity.z * DELTA_TIME;

			//Y軸でπ/2回転
			//これからはM_PIじゃなくてstd::numbers::pi_vを使おうね
			backToFrontMatrix = Matrix4x4Calculation::MakeRotateYMatrix(std::numbers::pi_v<float>);

			//カメラの回転を適用する
			billBoardMatrix = Matrix4x4Calculation::Multiply(backToFrontMatrix, camera.worldMatrix_);
			//平行成分はいらないよ
			//あくまで回転だけ
			billBoardMatrix.m[3][0] = 0.0f;
			billBoardMatrix.m[3][1] = 0.0f;
			billBoardMatrix.m[3][2] = 0.0f;

			//行列を作っていくよ
			scaleMatrix = Matrix4x4Calculation::MakeScaleMatrix(particleIterator->transform.scale);
			translateMatrix = Matrix4x4Calculation::MakeTranslateMatrix(particleIterator->transform.translate);


			//パーティクル個別のRotateは関係ないよ
			//その代わりにさっき作ったbillBoardMatrixを入れるよ
			worldMatrix = Matrix4x4Calculation::Multiply(scaleMatrix, Matrix4x4Calculation::Multiply(billBoardMatrix, translateMatrix));

			//最大値を超えて描画しないようにする
			if (numInstance_ < MAX_INSTANCE_NUMBER_) {
				instancingData_[numInstance_].World = worldMatrix;
				instancingData_[numInstance_].color = particleIterator->color;

				//透明になっていくようにするかどうか
				if (isToTransparent_ == true) {
					//アルファはVector4でのwだね
					float alpha = 1.0f - (particleIterator->currentTime / particleIterator->lifeTime);
					instancingData_[numInstance_].color.w = alpha;

				}

				++numInstance_;
			}

			break;


			#pragma endregion


		case ThrowUp:

			//強制的にビルボードにするよ
			float accel = -0.001f;
			velocityY_ += accel;


			particleIterator->transform.translate.x += particleIterator->velocity.x/3.0f;
			particleIterator->transform.translate.y += velocityY_;
			particleIterator->transform.translate.z += particleIterator->velocity.z / 3.0f;

			//Y軸でπ/2回転
			//これからはM_PIじゃなくてstd::numbers::pi_vを使おうね
			backToFrontMatrix = Matrix4x4Calculation::MakeRotateYMatrix(std::numbers::pi_v<float>);

			//カメラの回転を適用する
			billBoardMatrix = Matrix4x4Calculation::Multiply(backToFrontMatrix, camera.worldMatrix_);
			//平行成分はいらないよ
			//あくまで回転だけ
			billBoardMatrix.m[3][0] = 0.0f;
			billBoardMatrix.m[3][1] = 0.0f;
			billBoardMatrix.m[3][2] = 0.0f;

			//行列を作っていくよ
			scaleMatrix = Matrix4x4Calculation::MakeScaleMatrix(particleIterator->transform.scale);
			translateMatrix = Matrix4x4Calculation::MakeTranslateMatrix(particleIterator->transform.translate);


			//パーティクル個別のRotateは関係ないよ
			//その代わりにさっき作ったbillBoardMatrixを入れるよ
			worldMatrix = Matrix4x4Calculation::Multiply(scaleMatrix, Matrix4x4Calculation::Multiply(billBoardMatrix, translateMatrix));

			//最大値を超えて描画しないようにする
			if (numInstance_ < MAX_INSTANCE_NUMBER_) {
				instancingData_[numInstance_].World = worldMatrix;
				instancingData_[numInstance_].color = particleIterator->color;

				//ワールド座標
				Vector3 worldPosition = {
					.x = worldMatrix.m[3][0],
					.y = worldMatrix.m[3][1],
					.z = worldMatrix.m[3][2]
				};

				//0より小さくなったら透明
				if (worldPosition.y<0.0f) {
					//アルファはVector4でのwだね
					instancingData_[numInstance_].color.w = 0.0f;
				
				}

				++numInstance_;
			}
			break;

		}

		
	}
}

void Particle3D::Draw(Camera& camera, Material& material, DirectionalLight& directionalLight){
	
	//Directionalではなかったらassert
	if (material.lightingKinds_ != Directional) {
		assert(0);
	}
	
	//更新
	Update(camera);





	//コマンドを積む
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetParticle3DRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetParticle3DGraphicsPipelineState().Get());



	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//CBVを設定する
	//マテリアル
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, material.bufferResource_->GetGPUVirtualAddress());

	//インスタンシング
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, instancingIndex_);


	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	if (textureHandle_ != 0) {
		TextureManager::GraphicsCommand(2, textureHandle_);
	}

	//カメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, camera.bufferResource_->GetGPUVirtualAddress());


	//DirectionalLight
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, directionalLight.bufferResource_->GetGPUVirtualAddress());


	//DrawCall
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(UINT(vertices_.size()), numInstance_, 0, 0);

}


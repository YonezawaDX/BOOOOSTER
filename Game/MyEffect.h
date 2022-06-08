// MyEffect.h

#pragma once

#include "StepTimer.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>

class MyEffect
{
protected:

	DX::StepTimer m_timer;

	DirectX::SimpleMath::Matrix mWorld;
	DirectX::SimpleMath::Matrix mView;
	DirectX::SimpleMath::Matrix mProj;

	// エフェクト
	DirectX::AlphaTestEffect* mBatchEffect;

	// プリミティブバッチ
	DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* mBatch;

	// 入力レイアウト
	ID3D11InputLayout* mInputLayout;

	std::unique_ptr<DirectX::CommonStates> mStates;

	// テクスチャハンドル
	ID3D11ShaderResourceView* mTexture;

	// カメラ
	DirectX::SimpleMath::Vector3		mCamera;

	// 位置、方向、ライフ
	DirectX::SimpleMath::Vector3		mPosition;
	DirectX::SimpleMath::Vector3		mVelocity;
	float								mLife;

	// 位置、方向、ライフ（初期）
	DirectX::SimpleMath::Vector3		mStartPosition;
	DirectX::SimpleMath::Vector3		mStartVelocity;
	float								mStartLife;

	// 待ち時間
	float mWait;
public:

	//マネージャ管理のため共通項目を外部から貰えるように変更
	void Create(ID3D11ShaderResourceView* texture, DirectX::AlphaTestEffect* batchEffect, DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* batch, ID3D11InputLayout* inputLayout);

	// 初期化
	void Initialize(float life = 100.0f, DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3 velocity = DirectX::SimpleMath::Vector3::Zero);

	void Reset();

	// 更新
	void Update(DX::StepTimer timer);

	// 描画
	void Render();

	// 描画情報の設定
	void SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	
	// 方向を指定する
	void SetVelocity(DirectX::SimpleMath::Vector3 vel) { mVelocity = vel; }

protected:

	// 描画の詳細処理
	void Draw();
};
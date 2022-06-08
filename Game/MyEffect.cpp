// MyEffect.cpp

#include "pch.h"
#include "MyEffect.h"

#include "DeviceResources.h"
#include "StepTimer.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;


// 生成
void MyEffect::Create(ID3D11ShaderResourceView* texture, AlphaTestEffect* batchEffect, DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* batch, ID3D11InputLayout* inputLayout)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11Device1* device = pDR->GetD3DDevice();

	// エフェクトの取得
	mBatchEffect = batchEffect;
	// 入力レイアウト取得
	mInputLayout = inputLayout;

	// プリミティブバッチの取得
	mBatch = batch;

	mStates = std::make_unique<CommonStates>(device);
	// テクスチャの取得
	mTexture = texture;
}

// 初期化
void MyEffect::Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 velocity)
{
	// 初期座標などを設定する
	mStartPosition = pos;
	mStartVelocity = velocity;
	mStartLife = life;

	Reset();
}

// リセットする
void MyEffect::Reset()
{
	mPosition = mStartPosition;
	mVelocity = mStartVelocity;
	mLife = mStartLife;
}

// 更新
void MyEffect::Update(DX::StepTimer timer)
{
	float time = float(m_timer.GetElapsedSeconds());
	m_timer = timer;

	mWait -= time;
	if (mWait > 0.0f)
	{
		return;
	}

	mPosition += mVelocity;

	mLife -= time;
	if (mLife < 0.0f)
	{
		Reset();
	}

	Vector3 len = mPosition - mStartPosition;

}

// 描画
void MyEffect::Render()
{
	mWorld = Matrix::CreateBillboard(mPosition, mCamera, Vector3::UnitY);
	Draw();
}

// 描画位置などの設定
void MyEffect::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	mCamera = camera;
	mView = view;
	mProj = proj;
}

//描画の詳細処理
void MyEffect::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	auto context = pDR->GetD3DDeviceContext();

	// TODO: Add your rendering code here.
	context;


	// 頂点情報
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(Vector3(0.5f, 0.5f, 0.0f), Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(-0.5f, 0.5f, 0.0f), Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(-0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(0.5f,-0.5f, 0.0f), Vector2(0.0f, 1.0f)),
	};
	// テクスチャサンプラーの設定（クランプテクスチャアドレッシングモード）
	ID3D11SamplerState* samplers[1] = { mStates->LinearClamp() };
	context->PSSetSamplers(0, 1, samplers);
	ID3D11BlendState* blendstate = mStates->NonPremultiplied();

	CD3D11_DEFAULT default_state;

	CD3D11_BLEND_DESC bddesc(default_state);

	blendstate->GetDesc(&bddesc);
	bddesc.AlphaToCoverageEnable = TRUE;
	pDR->GetD3DDevice()->CreateBlendState(&bddesc, &blendstate);

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(mStates->DepthDefault(), 0);
	// カリングは左周り
	context->RSSetState(mStates->CullCounterClockwise());
	// 不透明のみ描画する設定
	mBatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	mBatchEffect->SetReferenceAlpha(0);
	mBatchEffect->SetWorld(mWorld);
	mBatchEffect->SetView(mView);
	mBatchEffect->SetProjection(mProj);
	mBatchEffect->SetTexture(mTexture);
	mBatchEffect->Apply(context);
	context->IASetInputLayout(mInputLayout);

	// 半透明部分を描画
	mBatch->Begin();
	mBatch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	mBatch->End();

	blendstate->Release();
}
#include "pch.h"
#include "DeviceResources.h"
#include "EffectManager.h"

#include "MyEffect.h" 

#include "DeviceResources.h" 
#include "StepTimer.h" 
#include <SimpleMath.h> 
#include <Effects.h> 
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include <CommonStates.h>

// 定数-------------------------------------------------------------------
const float EffectManager::EFFECT_SPEED = 0.2f; // エフェクトの速度
//------------------------------------------------------------------------

using namespace DirectX;
using namespace DirectX::SimpleMath;

// エフェクトの生成
void EffectManager::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11Device1* device = pDR->GetD3DDevice();

	//バッチエフェクトの作成
	mBatchEffect = std::make_unique<AlphaTestEffect>(device);
	mBatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	mBatchEffect->SetReferenceAlpha(255);
	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	mBatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, mInputLayout.GetAddressOf());

	//プリミティブバッチの作成
	mBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());

	//テクスチャのロード
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/Smoke.png",
		nullptr,
		mTexture.GetAddressOf()
	);

	// エフェクトの量
	int count = 1;

	for (int i = 0; i < count; i++)
	{
		std::unique_ptr<MyEffect> effect = std::make_unique<MyEffect>();
		effect->Create(mTexture.Get(), mBatchEffect.get(), mBatch.get(), mInputLayout.Get());
		mEffectList.push_back(std::move(effect));
	}
}

// 初期化
void EffectManager::Initialize(float life, Vector3 pos)
{
	//life,pos,vel の値でm_effectを初期化する
	for (std::list<std::unique_ptr<MyEffect>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		Vector3 vel = Vector3(0.1f, 0.0f, 0);

		(*ite)->Initialize(life, pos, vel);
	}
}

// 更新
void EffectManager::Update(const DX::StepTimer& timer)
{
	//timerを渡してm_effectの更新処理を行う
	for (std::list<std::unique_ptr<MyEffect>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		// プレイヤの状態によってエフェクトの方向を変えている
		if (mEffectDirection == Player::ePlayerDirection::BOOST_LEFT)
		{
			(*ite)->SetVelocity(Vector3(EFFECT_SPEED, 0.0f, 0.0f));
		}
		else if (mEffectDirection == Player::ePlayerDirection::BOOST_RIGHT)
		{
			(*ite)->SetVelocity(Vector3(-EFFECT_SPEED, 0.0f, 0.0f));
		}
		else if (mEffectDirection == Player::ePlayerDirection::BOOST_TOP)
		{
			(*ite)->SetVelocity(Vector3(0.0f, -EFFECT_SPEED, 0.0f));
		}
		else if (mEffectDirection == Player::ePlayerDirection::BOOST_DOWN)
		{
			(*ite)->SetVelocity(Vector3(0.0f, EFFECT_SPEED, 0.0f));
		}
		

		(*ite)->Update(timer);
	}
}

// 描画
void EffectManager::Render()
{
	if (static_cast<int>(mEffectDirection) <= 2)
		return;
	//m_effectを描画する
	//mEffect->Render();
	for (std::list<std::unique_ptr<MyEffect>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		(*ite)->Render();
	}
}

// 描画位置の設定など
void EffectManager::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	//camera,view,proj,の値をm_effectに渡す
	for (std::list<std::unique_ptr<MyEffect>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		(*ite)->SetRenderState(camera, view, proj);
	}
}

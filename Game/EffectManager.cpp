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

// �萔-------------------------------------------------------------------
const float EffectManager::EFFECT_SPEED = 0.2f; // �G�t�F�N�g�̑��x
//------------------------------------------------------------------------

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �G�t�F�N�g�̐���
void EffectManager::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11Device1* device = pDR->GetD3DDevice();

	//�o�b�`�G�t�F�N�g�̍쐬
	mBatchEffect = std::make_unique<AlphaTestEffect>(device);
	mBatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	mBatchEffect->SetReferenceAlpha(255);
	// ���̓��C�A�E�g����
	void const* shaderByteCode;
	size_t byteCodeLength;
	mBatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, mInputLayout.GetAddressOf());

	//�v���~�e�B�u�o�b�`�̍쐬
	mBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());

	//�e�N�X�`���̃��[�h
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/Smoke.png",
		nullptr,
		mTexture.GetAddressOf()
	);

	// �G�t�F�N�g�̗�
	int count = 1;

	for (int i = 0; i < count; i++)
	{
		std::unique_ptr<MyEffect> effect = std::make_unique<MyEffect>();
		effect->Create(mTexture.Get(), mBatchEffect.get(), mBatch.get(), mInputLayout.Get());
		mEffectList.push_back(std::move(effect));
	}
}

// ������
void EffectManager::Initialize(float life, Vector3 pos)
{
	//life,pos,vel �̒l��m_effect������������
	for (std::list<std::unique_ptr<MyEffect>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		Vector3 vel = Vector3(0.1f, 0.0f, 0);

		(*ite)->Initialize(life, pos, vel);
	}
}

// �X�V
void EffectManager::Update(const DX::StepTimer& timer)
{
	//timer��n����m_effect�̍X�V�������s��
	for (std::list<std::unique_ptr<MyEffect>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		// �v���C���̏�Ԃɂ���ăG�t�F�N�g�̕�����ς��Ă���
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

// �`��
void EffectManager::Render()
{
	if (static_cast<int>(mEffectDirection) <= 2)
		return;
	//m_effect��`�悷��
	//mEffect->Render();
	for (std::list<std::unique_ptr<MyEffect>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		(*ite)->Render();
	}
}

// �`��ʒu�̐ݒ�Ȃ�
void EffectManager::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	//camera,view,proj,�̒l��m_effect�ɓn��
	for (std::list<std::unique_ptr<MyEffect>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		(*ite)->SetRenderState(camera, view, proj);
	}
}

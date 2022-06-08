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


// ����
void MyEffect::Create(ID3D11ShaderResourceView* texture, AlphaTestEffect* batchEffect, DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* batch, ID3D11InputLayout* inputLayout)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11Device1* device = pDR->GetD3DDevice();

	// �G�t�F�N�g�̎擾
	mBatchEffect = batchEffect;
	// ���̓��C�A�E�g�擾
	mInputLayout = inputLayout;

	// �v���~�e�B�u�o�b�`�̎擾
	mBatch = batch;

	mStates = std::make_unique<CommonStates>(device);
	// �e�N�X�`���̎擾
	mTexture = texture;
}

// ������
void MyEffect::Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 velocity)
{
	// �������W�Ȃǂ�ݒ肷��
	mStartPosition = pos;
	mStartVelocity = velocity;
	mStartLife = life;

	Reset();
}

// ���Z�b�g����
void MyEffect::Reset()
{
	mPosition = mStartPosition;
	mVelocity = mStartVelocity;
	mLife = mStartLife;
}

// �X�V
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

// �`��
void MyEffect::Render()
{
	mWorld = Matrix::CreateBillboard(mPosition, mCamera, Vector3::UnitY);
	Draw();
}

// �`��ʒu�Ȃǂ̐ݒ�
void MyEffect::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	mCamera = camera;
	mView = view;
	mProj = proj;
}

//�`��̏ڍ׏���
void MyEffect::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	auto context = pDR->GetD3DDeviceContext();

	// TODO: Add your rendering code here.
	context;


	// ���_���
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(Vector3(0.5f, 0.5f, 0.0f), Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(-0.5f, 0.5f, 0.0f), Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(-0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(0.5f,-0.5f, 0.0f), Vector2(0.0f, 1.0f)),
	};
	// �e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j
	ID3D11SamplerState* samplers[1] = { mStates->LinearClamp() };
	context->PSSetSamplers(0, 1, samplers);
	ID3D11BlendState* blendstate = mStates->NonPremultiplied();

	CD3D11_DEFAULT default_state;

	CD3D11_BLEND_DESC bddesc(default_state);

	blendstate->GetDesc(&bddesc);
	bddesc.AlphaToCoverageEnable = TRUE;
	pDR->GetD3DDevice()->CreateBlendState(&bddesc, &blendstate);

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(mStates->DepthDefault(), 0);
	// �J�����O�͍�����
	context->RSSetState(mStates->CullCounterClockwise());
	// �s�����̂ݕ`�悷��ݒ�
	mBatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	mBatchEffect->SetReferenceAlpha(0);
	mBatchEffect->SetWorld(mWorld);
	mBatchEffect->SetView(mView);
	mBatchEffect->SetProjection(mProj);
	mBatchEffect->SetTexture(mTexture);
	mBatchEffect->Apply(context);
	context->IASetInputLayout(mInputLayout);

	// ������������`��
	mBatch->Begin();
	mBatch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	mBatch->End();

	blendstate->Release();
}
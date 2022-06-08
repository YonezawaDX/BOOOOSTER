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

	// �G�t�F�N�g
	DirectX::AlphaTestEffect* mBatchEffect;

	// �v���~�e�B�u�o�b�`
	DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* mBatch;

	// ���̓��C�A�E�g
	ID3D11InputLayout* mInputLayout;

	std::unique_ptr<DirectX::CommonStates> mStates;

	// �e�N�X�`���n���h��
	ID3D11ShaderResourceView* mTexture;

	// �J����
	DirectX::SimpleMath::Vector3		mCamera;

	// �ʒu�A�����A���C�t
	DirectX::SimpleMath::Vector3		mPosition;
	DirectX::SimpleMath::Vector3		mVelocity;
	float								mLife;

	// �ʒu�A�����A���C�t�i�����j
	DirectX::SimpleMath::Vector3		mStartPosition;
	DirectX::SimpleMath::Vector3		mStartVelocity;
	float								mStartLife;

	// �҂�����
	float mWait;
public:

	//�}�l�[�W���Ǘ��̂��ߋ��ʍ��ڂ��O������Ⴆ��悤�ɕύX
	void Create(ID3D11ShaderResourceView* texture, DirectX::AlphaTestEffect* batchEffect, DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* batch, ID3D11InputLayout* inputLayout);

	// ������
	void Initialize(float life = 100.0f, DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3 velocity = DirectX::SimpleMath::Vector3::Zero);

	void Reset();

	// �X�V
	void Update(DX::StepTimer timer);

	// �`��
	void Render();

	// �`����̐ݒ�
	void SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	
	// �������w�肷��
	void SetVelocity(DirectX::SimpleMath::Vector3 vel) { mVelocity = vel; }

protected:

	// �`��̏ڍ׏���
	void Draw();
};
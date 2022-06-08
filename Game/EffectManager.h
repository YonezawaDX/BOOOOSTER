//	�G�t�F�N�g�Ǘ��N���X	
//	EffectManager.h
#pragma once

#include "StepTimer.h"

#include <SimpleMath.h>
#include <list>
#include <WICTextureLoader.h>

#include "MyEffect.h"
#include "PlayScene/Player.h"

class EffectManager
{
public:
	// �萔-----------------------------------------
	static const float EFFECT_SPEED; // �G�t�F�N�g�̑��x
	//---------------------------------------------
public:
	// �G�t�F�N�g�̐���
	void Create();

	//Create()�őS�������͒����̂ŕ���
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos);

	// �X�V
	void Update(const DX::StepTimer& timer);

	// �`��
	void Render();

	// �`��ʒu�̕ύX�Ȃ�
	void SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	
	// �G�t�F�N�g�̌�����ύX����
	void SetEffectDirection(Player::ePlayerDirection playerDirection) { mEffectDirection = playerDirection; }

private:
	//�o�b�`�G�t�F�N�g
	std::unique_ptr<DirectX::AlphaTestEffect> mBatchEffect;

	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> mBatch;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;

	//�G�t�F�N�g�̃|�C���^
	//MyEffect* mEffect;
	std::list<std::unique_ptr<MyEffect>> mEffectList;

	//�e�N�X�`���f�[�^
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mTexture;

	// �G�t�F�N�g�̌�����ݒ肷��
	Player::ePlayerDirection mEffectDirection;
};
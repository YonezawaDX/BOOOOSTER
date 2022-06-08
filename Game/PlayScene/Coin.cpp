/*
�R�C���N���X
*/
#include "pch.h"
#include "Coin.h"
#include "Model3D.h"
#include "Player.h"
#include "Stage.h"

// �萔��ݒ�-------------------------------
const float Coin::MODEL_ROTATE_SPEED = 200.0f;
//------------------------------------------
// �R���X�g���N�^
Coin::Coin()
	:mPos(DirectX::SimpleMath::Vector3::Zero)
	,mCoinWorld(DirectX::SimpleMath::Matrix::Identity)
{
}

// �f�X�g���N�^
Coin::~Coin()
{
}

// ������
void Coin::Initialize(DirectX::SimpleMath::Vector3 pos)
{
	// ���f�������
	mCoinModel3D = std::make_unique<Model3D>();
	mCoinModel3D->Initialize(L"Resources/Models/CoinGame.cmo");

	// ���W������������炤
	mPos = pos;

	//���[���h���W��ݒ肷��
	mCoinWorld = DirectX::SimpleMath::Matrix::CreateTranslation(mPos);
}

// �X�V
void Coin::Update(float time)
{
	// �R�C������]������
	mCoinModel3D->SetModelRotationY(time * MODEL_ROTATE_SPEED);
}

// �`��
void Coin::Draw(DirectX::CommonStates* commonStates, FollowCamera* camera)
{
	// �R�C����`�悷��
	mCoinModel3D->Draw(commonStates, &mCoinWorld, camera);
}

// �I������
void Coin::Finalize()
{
}

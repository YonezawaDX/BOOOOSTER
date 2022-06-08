/*
�v���C�V�[��
*/
#include "pch.h"

#include "../GameMain.h"
#include "DeviceResources.h"

#include "Libraries/MyLibraries/GridFloor.h"
#include "Libraries/MyLibraries/Camera.h"
#include "Libraries/MyLibraries/FollowCamera.h"
#include "Libraries/MyLibraries/Circle.h"

#include "PlayScene.h"
#include "Stage.h"
#include "Player.h"
#include "Coin.h"
#include "Game/GameData.h"



// ���O��Ԃ̗��p
using namespace DirectX;
using namespace DirectX::SimpleMath;

// �R���X�g���N�^
PlayScene::PlayScene()
	: mFps(0)
	,mCoin{}
	,mCoinFlag{}
	,mCoinCount(0.0f)
	,mFadeEnd(false)
{
}

// �f�X�g���N�^
PlayScene::~PlayScene()
{
}

// ������
void PlayScene::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	auto device = pDR->GetD3DDevice();
	auto context = pDR->GetD3DDeviceContext();

	// �R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	mCommonState = std::make_unique<DirectX::CommonStates>(device);

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	mSpriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	// �v���~�e�B�u�o�b�`
	mPrimitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);

	// �x�[�V�b�N�G�t�F�N�g
	mBasicEffect = std::make_unique<DirectX::BasicEffect>(device);

	// ���_�J���[�̗L����
	mBasicEffect->SetVertexColorEnabled(true);

	// �C���v�b�g���C�A�E�g�̐ݒ�
	void const* shaderByteCode;
	size_t byteCodeLength;
	mBasicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		mInputLayout.ReleaseAndGetAddressOf()
	);


	// ���f���f�[�^�̓����Ă���t�H���_�̎w��
	std::unique_ptr<EffectFactory> factory = std::make_unique<EffectFactory>(device);
	factory->SetDirectory(L"Resources/Models");

	// ���f���̃��[�h
	mModel = Model::CreateFromCMO(
		device,
		L"Resources/Models/BackGroundSphere.cmo",	// �T�C�R�����f���𗘗p�����
		*factory
	);

	//�w�i�p���f���̏�����
	mModelWorld = SimpleMath::Matrix::Identity;
	//�X�e�[�W�̐���
	mStage = std::make_unique<Stage>();

	// �J�����̍쐬
	mCamera = std::make_unique<Camera>();
	mFollowCamera = std::make_unique<FollowCamera>();

	// �v���C���[�̐���
	mPlayer = std::make_unique<Player>(mStage.get());
	mPlayer->Initialize();

	// �}�b�v��ɃR�C���𐶐�����
	for (int i = 0; i < mStage->GetMapRow(); i++)
	{
		for (int j = 0; j < mStage->GetMapColumn(); j++)
		{
			// �u���b�N�̕`��ʒu�����炷
			DirectX::SimpleMath::Vector3 pos(j + 0.5f, -i - 0.5f, 0.0f);

			// �\������3D�I�u�W�F�N�g�̎��
			if(mStage->GetMap()[i][j] == Stage::eMapType::Coin)
			{
				mCoin.push_back(std::make_unique<Coin>());
				if (!mCoin.empty())
				{
					mCoin[mCoin.size() - 1]->Initialize(pos);
					mCoinFlag.push_back(true);
				}
			}
		}
	}

	mEffect = std::make_unique<ShaderManager>();
	mEffect->Create();
	mEffect->SetFade(true);
	mEffect->Initialize(100.0f, Vector3(0.0f, 0.0f, 0.0f));
	mEffect->SetRenderState(
		mFollowCamera->GetTargetPosition() + Vector3(0.0f, 2.0f, 18.0f),
		mFollowCamera->GetEyePosition() + Vector3(0.0f, -15.0f, 100.0f),
		mFollowCamera->GetViewMatrix(),
		mFollowCamera->GetProjectionMatrix()
	);
	
}

// �X�V
// �߂�l	:���̃V�[���ԍ�
GAME_SCENE PlayScene::Update(const DX::StepTimer& timer)
{
	if (mFadeEnd == false)
	{
		mEffect->SetFade(false);
		mEffect->Update(timer);
	}

	if (mEffect->GetDarkScreen())
	{
		return GAME_SCENE::NONE;
	}
	// �t���[�����[�g�̎擾
	mFps = timer.GetFramesPerSecond();
	float time = float(timer.GetTotalSeconds());

	// �J�����̐ݒ�
	// �v���C���[�̌����ɂ���ăJ�����̈ʒu��ύX����
	mFollowCamera->SetPlayerDirection(mPlayer->GetPlayerDirection());
	mFollowCamera->SetRefTargetPosition(mPlayer->GetPlayerPos());
	mFollowCamera->SetRefEyePosition(mPlayer->GetPlayerPos() + mFollowCamera->TARGET_TO_EYE_VEC);
	// �J�����̍X�V
	mFollowCamera->Update();

	// �v���C���[�̍X�V
	mPlayer->Update(time,timer);
	
	// �R�C���̍X�V(��]������)
	for (unsigned int i = 0; i < mCoin.size(); i++)
	{
		if (mCoin[i] != nullptr)
		{
			mCoin[i]->Update(time);
		}
	}

	// �R�C���Ƃ̔���
	bool no = true;
    for (unsigned int i = 0;i < mCoin.size(); i++)
    {
		if (mCoin[i] != nullptr)
		{
			// �v���C���[�Ƃ̔�������
			no = mPlayer->GetPlayerPos().x + 0.5f < mCoin[i]->GetCoinPos().x - 0.5f ||
				 mPlayer->GetPlayerPos().y + 0.5f < mCoin[i]->GetCoinPos().y - 0.5f ||
				 mCoin[i]->GetCoinPos().x + 0.5f  < mPlayer->GetPlayerPos().x - 0.5f ||
				 mCoin[i]->GetCoinPos().y + 0.5f  < mPlayer->GetPlayerPos().y - 0.5f ;
		}
	    if (!no)
		{
			// �v���C���[�Ɠ��������ꍇ���̃R�C�����폜
			mCoin[i].reset(nullptr);
			mCoinFlag[i] = false;
			mCoinCount++;
	    }
	     
	}

	// �X�e�[�W�̍X�V
	mStage->Update(time);

	bool gool = true;
	gool = mPlayer->GetPlayerPos().x + 0.5f < mStage->GetGoolPos().x - 0.5f ||
		mPlayer->GetPlayerPos().y + 0.5f < mStage->GetGoolPos().y - 0.5f ||
		mStage->GetGoolPos().x + 0.5f < mPlayer->GetPlayerPos().x - 0.5f ||
		mStage->GetGoolPos().y + 0.5f < mPlayer->GetPlayerPos().y - 0.5f;
	// �S�[������
	if (mPlayer->GetPlayerLife() < 0.0f || !gool)
	{
		mFadeEnd = true;
	}

	// �t�F�[�h�C���̏���
	if (mFadeEnd)
	{
		mEffect->SetFade(true);
		mEffect->Update(timer);
		if (mEffect->GetDarkScreen())
		{
			return GAME_SCENE::RESULT;
		}
	}

	
	return GAME_SCENE::NONE;
}

// �`��
void PlayScene::Draw()
{
	// �f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();


	// �f�o�C�X�R���e�L�X�g�̎擾
	auto context = pDR->GetD3DDeviceContext();

	// �ϊ��s��̐錾(���[���h�A�r���[�A�ˉe)
	SimpleMath::Matrix world, view, projection;

	// ���[���h�s��̏�����(�P�ʍs��)
	world = SimpleMath::Matrix::Identity;

	// �ˉe�s��̎擾
	projection = mFollowCamera->GetProjectionMatrix();

	// �r���[�s��̎擾
	view = mFollowCamera->GetViewMatrix();

	// �X�e�[�W�̕`��
	mStage->Draw(mCommonState.get(), mFollowCamera.get());

	// �w�i�̕`��
	mModel->Draw(
		context,
		*mCommonState,
		mModelWorld,
		view,
		projection
	);

	// �R�C���̕`��
	int i = 0;
	for (auto& c : mCoin)
	{
		if (mCoinFlag[i])
		{
			c->Draw(mCommonState.get(), mFollowCamera.get());
		}
		i++;
	}

	// FPS�̎擾�ƕ\��������̏���
	wchar_t buf[32];
	swprintf_s(buf, 32, L"FPS = %d", mFps);

	// �X�v���C�g�o�b�`�ɂ��X�R�A�̕\��
	mSpriteBatch->Begin();
	// �X�R�A�̕\��--------------------------------------------------------------------
	swprintf_s(buf, 32, L"Score : %d", g_gameData.gScore);
	mSpriteFont->DrawString(mSpriteBatch.get(), buf, DirectX::XMFLOAT2(500, 10));
	g_gameData.gScore = static_cast<int>(mCoinCount * 100.0f);
	//-----------------------------------------------------------------------------------
	mSpriteBatch->End();

	// �v���C���[�̕`��
	mPlayer->Draw(mCommonState.get(),mFollowCamera.get());


	// �ȉ�UI�̕`��-------------------------------------------------------------------------
	// �x�[�V�b�N�G�t�F�N�g�̐ݒ�
	mBasicEffect->SetWorld(world);
	mBasicEffect->SetView(view);
	mBasicEffect->SetProjection(projection);
	mBasicEffect->Apply(context);

	// ���̓��C�A�E�g�̐ݒ�F�|���S���̒��_�^�ɂ���ē��̓��C�A�E�g�̒��g���ς��
	context->IASetInputLayout(mInputLayout.Get());

	// �u�����h�X�e�[�g�̐ݒ�F�A���t�@�u�����f�B���O�̕`����@�̎w��
	context->OMSetBlendState(mCommonState->NonPremultiplied(), nullptr, 0xffffffff);

	// �[�x�X�e���V���X�e�[�g�̐ݒ�F�[�x�X�e���V���o�b�t�@�̎g�����̎w��
	context->OMSetDepthStencilState(mCommonState->DepthDefault(), 0);

	// Circle�I�u�W�F�N�g�̃��[�J���ϐ��̎w��
	Circle circle1;
	DirectX::SimpleMath::Vector3 pos1(mFollowCamera->GetTargetPosition() + DirectX::SimpleMath::Vector3(-4.5f, 2.5f, 0.0f));
	DirectX::SimpleMath::Vector3 pos2(mFollowCamera->GetTargetPosition()+ DirectX::SimpleMath::Vector3(-4.5f,3.5f,0.0f));

	// �v���~�e�B�u�o�b�`�ɂ��|���S���̕`��
	mPrimitiveBatch->Begin();
	// �p�����[�^�F�v���~�e�B�u�o�b�`�A���S���W�A���a�A�~��(0.0f-1.0f)�A�F�A���߂̃A���t�@�l
	circle1.Draw(mPrimitiveBatch.get(), pos1, 0.3f, mPlayer->GetBoostTime()/100.0f, DirectX::Colors::Yellow, 0.5f);
	// �p�����[�^�F�v���~�e�B�u�o�b�`�A���S���W�A���a�A�~��(0.0f-1.0f)�A�F�A���߂̃A���t�@�l
	circle1.Draw(mPrimitiveBatch.get(), pos2, 0.3f, mPlayer->GetPlayerLife() / 100.0f, DirectX::Colors::Red, 0.5f);
	mPrimitiveBatch->End();

	mEffect->Render();
}


// �I������
void PlayScene::Finalize()
{
}

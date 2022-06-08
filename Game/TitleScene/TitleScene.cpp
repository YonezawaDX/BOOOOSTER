/*
�^�C�g���V�[��
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"

#include "TitleScene.h"
#include "Game/GameData.h"

// �萔--------------------------------------------
const int TitleScene::PUTH_Z_X = 350;		// ������X���W
const int TitleScene::PUTH_Z_Y = 100;		// ������Y���W

const float TitleScene::TITLE_BACK_X = 0.0f;		// �w�i��X���W
const float TitleScene::TITLE_BACK_Y = -100.0f;		// �w�i��Y���W

const float TitleScene::TITLE_X = 400.0f - 128.0f;		// title������X���W
const float TitleScene::TITLE_Y = 300.0f - 128.0f;		// title������Y���W
//-----------------------------------------------

using namespace DirectX;

// �R���X�g���N�^
TitleScene::TitleScene()
{
}

// �f�X�g���N�^
TitleScene::~TitleScene()
{
}

// ������
void TitleScene::Initialize()
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

	// �e�N�X�`���̓ǂݍ��݁iTitle�j
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/Title.png",
		nullptr,
		mTexture.ReleaseAndGetAddressOf()
	);
	// �e�N�X�`���̓ǂݍ��݁iTitle�̔w�i�j
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/tititi.png",
		nullptr,
		mTexture2.ReleaseAndGetAddressOf()
	);

	// �X�R�A�̏�����
	g_gameData.Initialize();

	//�G�t�F�N�g
	mEffect = std::make_unique<ShaderManager>();
	mEffect->Create();

	DirectX::SimpleMath::Vector3 camera = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);
	DirectX::SimpleMath::Vector3 target = DirectX::SimpleMath::Vector3(0, 0, 0);

	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::CreateLookAt(camera, target, DirectX::SimpleMath::Vector3::UnitY);

	RECT outputSize = pDR->GetOutputSize();
	UINT backBufferWidth = std::max<UINT>(outputSize.right - outputSize.left, 1);
	UINT backBufferHeight = std::max<UINT>(outputSize.bottom - outputSize.top, 1);

	DirectX::SimpleMath::Matrix proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(backBufferWidth) / float(backBufferHeight), 0.1f, 100.f);

	mEffect->SetRenderState(target, camera, view, proj);
	mEffect->Initialize(100.0f, DirectX::SimpleMath::Vector3(10.0f, 10.0f, 0.0f));
	mEffect->SetFade(false);
}

// �X�V
// �߂�l	:���̃V�[���ԍ�
GAME_SCENE TitleScene::Update(const DX::StepTimer& timer)
{
	// �x���悯
	UNREFERENCED_PARAMETER(timer);

	// �L�[���͏����擾����
	auto keyState = DirectX::Keyboard::Get().GetState();

	mEffect->Update(timer);

	if (keyState.Z)
	{
		mEffect->SetFade(true);
	}

	if (mEffect->GetDarkScreen())
	{
		return GAME_SCENE::PLAY;
	}

	return GAME_SCENE::NONE;
}

// �`��
void TitleScene::Draw()
{
	mSpriteBatch->Begin(SpriteSortMode_Deferred, mCommonState->NonPremultiplied());

	// Title�̕`��
	mSpriteBatch->Draw(
		mTexture2.Get(),
		SimpleMath::Vector2(TitleScene::TITLE_BACK_X, TitleScene::TITLE_BACK_Y)
	);
	mSpriteBatch->Draw(
		mTexture.Get(),
		SimpleMath::Vector2 (TitleScene::TITLE_X, TitleScene::TITLE_Y)
	);

	mSpriteFont->DrawString(
		mSpriteBatch.get(),
		L"Push Z Key",
		DirectX::XMFLOAT2(TitleScene::PUTH_Z_X, TitleScene::PUTH_Z_Y));

	mSpriteBatch->End();

	mEffect->Render();
}

// �I������
void TitleScene::Finalize()
{
}

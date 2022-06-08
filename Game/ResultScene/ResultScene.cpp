/*
���U���g�V�[��
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"

#include "ResultScene.h"
#include "Game/GameData.h"

using namespace DirectX;

// �R���X�g���N�^
ResultScene::ResultScene()
{
}

// �f�X�g���N�^
ResultScene::~ResultScene()
{
}

// ������
void ResultScene::Initialize()
{
	// �f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	auto device = pDR->GetD3DDevice();
	auto context = pDR->GetD3DDeviceContext();

	// �R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	mCommonState = std::make_unique<DirectX::CommonStates>(device);

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	mSpriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	// �e�N�X�`���̓ǂݍ���
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/gool.png",
		nullptr,
		mTexture.ReleaseAndGetAddressOf()
	);

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
	mEffect->SetFade(true);
	mEffect->Initialize(100.0f, DirectX::SimpleMath::Vector3(10.0f, 10.0f, 0.0f));
	mEffect->SetFade(false);
}


// �X�V
// �߂�l	:���̃V�[���ԍ�
GAME_SCENE ResultScene::Update(const DX::StepTimer& timer)
{
	// �x���悯
	UNREFERENCED_PARAMETER(timer);

	mEffect->Update(timer);
	// �L�[���͏����擾����
	auto keyState = DirectX::Keyboard::Get().GetState();

	// �}�E�X���͏����擾����
	auto mouseState = DirectX::Mouse::Get().GetState();

	if (keyState.Z || mouseState.leftButton)
	{
		return GAME_SCENE::TITLE;
	}

	return GAME_SCENE::NONE;
}

// �`��
void ResultScene::Draw()
{

	mSpriteBatch->Begin(SpriteSortMode_Deferred, mCommonState->NonPremultiplied());

	// �X�R�A�̕`��ϐ�
	SimpleMath::Vector2 pos(0,0);

	mSpriteBatch->Draw(mTexture.Get(), pos);

	// �X�R�A��`�悷��
	wchar_t buf[32];
	swprintf_s(buf, 32, L"Score :%d", g_gameData.gScore);
	mSpriteFont->DrawString(mSpriteBatch.get(), buf, DirectX::XMFLOAT2(500, 180));

	mSpriteBatch->End();

	mEffect->Render();
}


// �I������
void ResultScene::Finalize()
{
}

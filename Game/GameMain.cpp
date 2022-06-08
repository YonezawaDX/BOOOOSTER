//
// Scene.cpp
//
#include "pch.h"

#include "DeviceResources.h"
#include "GameMain.h"

#include "Libraries/MyLibraries/DebugCamera.h"
#include "Libraries/MyLibraries/GridFloor.h"

#include "Game/TitleScene/TitleScene.h"
#include "Game/PlayScene/PlayScene.h"
#include "Game/ResultScene/ResultScene.h"

// ���O��Ԃ̗��p
using namespace DirectX;

//-------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------
GameMain::GameMain()
	: mNextScene(GAME_SCENE::TITLE)		// �����V�[���̐ݒ�
	, mpScene(nullptr)
{
}

//-------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------
GameMain::~GameMain()
{
	Finalize();
}

//-------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------
void GameMain::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// �L�[�{�[�h�֘A
	mKeybord             = std::make_unique<DirectX::Keyboard>();

	// �}�E�X�֘A
	mMouse = std::make_unique<DirectX::Mouse>();
	mMouse->SetWindow(pDR->GetHwnd());

	// �V�[���쐬
	CreateScene();
}

//-------------------------------------------------------------------
// �X�V����
//-------------------------------------------------------------------
void GameMain::Update(const DX::StepTimer& timer)
{
	// �L�[���͏����擾����
	auto keyState = mKeybord->GetState();

	// ESC�L�[�ŏI��
	if (keyState.Escape) PostQuitMessage(0);

	// ���̃V�[�����ݒ肳��Ă�����V�[���؂�ւ�
	if (mNextScene != GAME_SCENE::NONE)
	{
		// �V�[���폜
		DeleteScene();
		
		// �V�[���쐬
		CreateScene();
	}

	// ���Ԃ�����΍X�V
	if (mpScene != nullptr)
	{
		mNextScene = mpScene->Update(timer);
	}
}

//-------------------------------------------------------------------
// �`�揈��
//-------------------------------------------------------------------
void GameMain::Render()
{
	// ���Ԃ�����Ε`��
	if (mpScene != nullptr)
	{
		mpScene->Draw();
	}
}

//-------------------------------------------------------------------
// ��n��
//-------------------------------------------------------------------
void GameMain::Finalize()
{
	DeleteScene();
}

/*--------------------------------------------------
�V�[���̍쐬
--------------------------------------------------*/
void GameMain::CreateScene()
{
	// �V�[�����쐬����Ă���Ƃ��͏������Ȃ�
	if (mpScene != nullptr)
	{
		return;
	}

	// ���V�[���̍쐬
	switch (mNextScene)
	{
	case GAME_SCENE::TITLE:
	{
		mpScene = new TitleScene();
		break;
	}
	case GAME_SCENE::PLAY:
	{
		mpScene = new PlayScene();
		break;
	}
	case GAME_SCENE::RESULT:
	{
		mpScene = new ResultScene();
		break;
	}
	default:
	{
		// ��O�Ȃ̂ŏ����𒆒f
		return;
	}
	}

	// �쐬�����V�[����������
	mpScene->Initialize();

	// ���ւ̃V�[������������
	mNextScene = GAME_SCENE::NONE;
}

/*--------------------------------------------------
�V�[���̍폜
--------------------------------------------------*/
void GameMain::DeleteScene()
{
	// �V�[�����쐬����Ă��Ȃ���Ώ������Ȃ�
	if (mpScene == nullptr)
	{
		return;
	}

	// ���V�[���̏I������
	mpScene->Finalize();

	// ���V�[���̍폜
	delete mpScene;
	mpScene = nullptr;
}

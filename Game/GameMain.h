//
// GameMain.h
//
#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include "StepTimer.h"

// �V�[���̗�
enum class GAME_SCENE : int
{
    NONE,

    TITLE,
    PLAY,
    RESULT
};

// �O���錾
class DebugCamera;

class IScene;

/// <summary>
/// Game�N���X����A���[�U���������𔲂��o�����N���X
/// </summary>
class GameMain
{
private:

    // �L�[�{�[�h
    std::unique_ptr<DirectX::Keyboard> mKeybord;
    
    // �}�E�X
    std::unique_ptr<DirectX::Mouse> mMouse;

	// ���̃V�[���ԍ�
	GAME_SCENE mNextScene;

	// �V�[��
	IScene* mpScene;

public:
    GameMain();
    ~GameMain();

    void Initialize();
    void Update(const DX::StepTimer& timer);
    void Render();
    void Finalize();

	// �V�[���֌W����

	// �V�[���̍쐬
	void CreateScene();

	// �V�[���̍폜
	void DeleteScene();
};

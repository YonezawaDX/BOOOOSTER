#include "pch.h"
#include "InputManager.h"

//---------------------------------------
// ����������
//---------------------------------------
void InputManager::Initialize(const HWND& window)
{
	// �L�[�{�[�h�֘A
	mKeyboard = std::make_unique<DirectX::Keyboard>();
	mKeyboardStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	
	// �}�E�X�֘A
	mMouse = std::make_unique<DirectX::Mouse>();
	mMouse->SetWindow(window);
	mButtonStateTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();

}

//---------------------------------------
// �X�V����
//---------------------------------------

void InputManager::Update()
{
	// �L�[�{�[�h�̏�ԍX�V
	mKeyboardState = mKeyboard->GetState();
	// �L�[�{�[�h�g���b�J�[�̍X�V
	mKeyboardStateTracker->Update(mKeyboardState);

	// �}�E�X�̏�ԍX�V
	mMouseState = mMouse->GetState();
	// �}�E�X�{�^���g���b�J�[�̍X�V
	mButtonStateTracker->Update(mMouseState);
}

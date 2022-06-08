#pragma once
#include <Mouse.h>
#include <Keyboard.h>

///
///�}�E�X�A�L�[�{�[�h�̓��͂𐿂������N���X
/// 
class InputManager
{
public:
	// �V���O���g��������ɂ������ć@
	// �C���X�^���X��Ԃ��֐�����������
	static InputManager* GetInstance()
	{
		static InputManager instance;
		return &instance;
	}
private:
	// �}�E�X�֘A
	std::unique_ptr<DirectX::Mouse> mMouse;
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> mButtonStateTracker;
	DirectX::Mouse::State mMouseState;
	// �L�[�{�[�h�֘A
	std::unique_ptr<DirectX::Keyboard> mKeyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> mKeyboardStateTracker;
	DirectX::Keyboard::State mKeyboardState;

private:
	// �V���O���g��������ɂ������ćA
	// �R���X�g���N�^�A�f�X�g���N�^��private��
	InputManager() : mKeyboardState(), mMouseState() {};
	~InputManager() = default;

private:
	// �V���O���g��������ɂ������ćB
	// �R�s�[�R���X�g���N�^�A���[�u�R���X�g���N�^�A�֌W���������Z�q�̍폜
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	InputManager(const InputManager&&) = delete;
	InputManager&& operator=(const InputManager&&) = delete;

public:
	// ����������
	void Initialize(const HWND& window);
	// �X�V����
	void Update();
	// �e��Q�b�^
	DirectX::Mouse::State& GetMouseState() { return mMouseState; }
	DirectX::Keyboard::State& GetKeyboardState() { return mKeyboardState; }
	DirectX::Mouse::ButtonStateTracker* GetButtonStateTracker() const { return mButtonStateTracker.get(); }
	DirectX::Keyboard::KeyboardStateTracker* GetKeyboardStateTracker()const { return mKeyboardStateTracker.get(); }

};
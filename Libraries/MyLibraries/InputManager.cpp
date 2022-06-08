#include "pch.h"
#include "InputManager.h"

//---------------------------------------
// 初期化処理
//---------------------------------------
void InputManager::Initialize(const HWND& window)
{
	// キーボード関連
	mKeyboard = std::make_unique<DirectX::Keyboard>();
	mKeyboardStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	
	// マウス関連
	mMouse = std::make_unique<DirectX::Mouse>();
	mMouse->SetWindow(window);
	mButtonStateTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();

}

//---------------------------------------
// 更新処理
//---------------------------------------

void InputManager::Update()
{
	// キーボードの状態更新
	mKeyboardState = mKeyboard->GetState();
	// キーボードトラッカーの更新
	mKeyboardStateTracker->Update(mKeyboardState);

	// マウスの状態更新
	mMouseState = mMouse->GetState();
	// マウスボタントラッカーの更新
	mButtonStateTracker->Update(mMouseState);
}

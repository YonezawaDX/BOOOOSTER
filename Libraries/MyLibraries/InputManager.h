#pragma once
#include <Mouse.h>
#include <Keyboard.h>

///
///マウス、キーボードの入力を請け負うクラス
/// 
class InputManager
{
public:
	// シングルトン化するにあたって①
	// インスタンスを返す関数を準備する
	static InputManager* GetInstance()
	{
		static InputManager instance;
		return &instance;
	}
private:
	// マウス関連
	std::unique_ptr<DirectX::Mouse> mMouse;
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> mButtonStateTracker;
	DirectX::Mouse::State mMouseState;
	// キーボード関連
	std::unique_ptr<DirectX::Keyboard> mKeyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> mKeyboardStateTracker;
	DirectX::Keyboard::State mKeyboardState;

private:
	// シングルトン化するにあたって②
	// コンストラクタ、デストラクタはprivate化
	InputManager() : mKeyboardState(), mMouseState() {};
	~InputManager() = default;

private:
	// シングルトン化するにあたって③
	// コピーコンストラクタ、ムーブコンストラクタ、関係する代入演算子の削除
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	InputManager(const InputManager&&) = delete;
	InputManager&& operator=(const InputManager&&) = delete;

public:
	// 初期化処理
	void Initialize(const HWND& window);
	// 更新処理
	void Update();
	// 各種ゲッタ
	DirectX::Mouse::State& GetMouseState() { return mMouseState; }
	DirectX::Keyboard::State& GetKeyboardState() { return mKeyboardState; }
	DirectX::Mouse::ButtonStateTracker* GetButtonStateTracker() const { return mButtonStateTracker.get(); }
	DirectX::Keyboard::KeyboardStateTracker* GetKeyboardStateTracker()const { return mKeyboardStateTracker.get(); }

};
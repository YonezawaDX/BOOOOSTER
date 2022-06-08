//
// GameMain.h
//
#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include "StepTimer.h"

// シーンの列挙
enum class GAME_SCENE : int
{
    NONE,

    TITLE,
    PLAY,
    RESULT
};

// 前方宣言
class DebugCamera;

class IScene;

/// <summary>
/// Gameクラスから、ユーザ処理部分を抜き出したクラス
/// </summary>
class GameMain
{
private:

    // キーボード
    std::unique_ptr<DirectX::Keyboard> mKeybord;
    
    // マウス
    std::unique_ptr<DirectX::Mouse> mMouse;

	// 次のシーン番号
	GAME_SCENE mNextScene;

	// シーン
	IScene* mpScene;

public:
    GameMain();
    ~GameMain();

    void Initialize();
    void Update(const DX::StepTimer& timer);
    void Render();
    void Finalize();

	// シーン関係処理

	// シーンの作成
	void CreateScene();

	// シーンの削除
	void DeleteScene();
};

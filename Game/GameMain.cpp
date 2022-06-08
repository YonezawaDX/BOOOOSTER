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

// 名前空間の利用
using namespace DirectX;

//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
GameMain::GameMain()
	: mNextScene(GAME_SCENE::TITLE)		// 初期シーンの設定
	, mpScene(nullptr)
{
}

//-------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------
GameMain::~GameMain()
{
	Finalize();
}

//-------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------
void GameMain::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// キーボード関連
	mKeybord             = std::make_unique<DirectX::Keyboard>();

	// マウス関連
	mMouse = std::make_unique<DirectX::Mouse>();
	mMouse->SetWindow(pDR->GetHwnd());

	// シーン作成
	CreateScene();
}

//-------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------
void GameMain::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	auto keyState = mKeybord->GetState();

	// ESCキーで終了
	if (keyState.Escape) PostQuitMessage(0);

	// 次のシーンが設定されていたらシーン切り替え
	if (mNextScene != GAME_SCENE::NONE)
	{
		// シーン削除
		DeleteScene();
		
		// シーン作成
		CreateScene();
	}

	// 実態があれば更新
	if (mpScene != nullptr)
	{
		mNextScene = mpScene->Update(timer);
	}
}

//-------------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------------
void GameMain::Render()
{
	// 実態があれば描画
	if (mpScene != nullptr)
	{
		mpScene->Draw();
	}
}

//-------------------------------------------------------------------
// 後始末
//-------------------------------------------------------------------
void GameMain::Finalize()
{
	DeleteScene();
}

/*--------------------------------------------------
シーンの作成
--------------------------------------------------*/
void GameMain::CreateScene()
{
	// シーンが作成されているときは処理しない
	if (mpScene != nullptr)
	{
		return;
	}

	// 次シーンの作成
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
		// 例外なので処理を中断
		return;
	}
	}

	// 作成したシーンを初期化
	mpScene->Initialize();

	// 次へのシーン情報を初期化
	mNextScene = GAME_SCENE::NONE;
}

/*--------------------------------------------------
シーンの削除
--------------------------------------------------*/
void GameMain::DeleteScene()
{
	// シーンが作成されていなければ処理しない
	if (mpScene == nullptr)
	{
		return;
	}

	// 現シーンの終了処理
	mpScene->Finalize();

	// 現シーンの削除
	delete mpScene;
	mpScene = nullptr;
}

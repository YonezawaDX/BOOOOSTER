/*
タイトルシーン
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"

#include "TitleScene.h"
#include "Game/GameData.h"

// 定数--------------------------------------------
const int TitleScene::PUTH_Z_X = 350;		// 文字のX座標
const int TitleScene::PUTH_Z_Y = 100;		// 文字のY座標

const float TitleScene::TITLE_BACK_X = 0.0f;		// 背景のX座標
const float TitleScene::TITLE_BACK_Y = -100.0f;		// 背景のY座標

const float TitleScene::TITLE_X = 400.0f - 128.0f;		// title文字のX座標
const float TitleScene::TITLE_Y = 300.0f - 128.0f;		// title文字のY座標
//-----------------------------------------------

using namespace DirectX;

// コンストラクタ
TitleScene::TitleScene()
{
}

// デストラクタ
TitleScene::~TitleScene()
{
}

// 初期化
void TitleScene::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3Dデバイスとデバイスコンテキストの取得
	auto device = pDR->GetD3DDevice();
	auto context = pDR->GetD3DDeviceContext();

	// コモンステート::D3Dレンダリング状態オブジェクト
	mCommonState = std::make_unique<DirectX::CommonStates>(device);

	// スプライトバッチ::デバッグ情報の表示に必要
	mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	mSpriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	// テクスチャの読み込み（Title）
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/Title.png",
		nullptr,
		mTexture.ReleaseAndGetAddressOf()
	);
	// テクスチャの読み込み（Titleの背景）
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/tititi.png",
		nullptr,
		mTexture2.ReleaseAndGetAddressOf()
	);

	// スコアの初期化
	g_gameData.Initialize();

	//エフェクト
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

// 更新
// 戻り値	:次のシーン番号
GAME_SCENE TitleScene::Update(const DX::StepTimer& timer)
{
	// 警告よけ
	UNREFERENCED_PARAMETER(timer);

	// キー入力情報を取得する
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

// 描画
void TitleScene::Draw()
{
	mSpriteBatch->Begin(SpriteSortMode_Deferred, mCommonState->NonPremultiplied());

	// Titleの描画
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

// 終了処理
void TitleScene::Finalize()
{
}

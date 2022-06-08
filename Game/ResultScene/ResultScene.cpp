/*
リザルトシーン
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"

#include "ResultScene.h"
#include "Game/GameData.h"

using namespace DirectX;

// コンストラクタ
ResultScene::ResultScene()
{
}

// デストラクタ
ResultScene::~ResultScene()
{
}

// 初期化
void ResultScene::Initialize()
{
	// デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3Dデバイスとデバイスコンテキストの取得
	auto device = pDR->GetD3DDevice();
	auto context = pDR->GetD3DDeviceContext();

	// コモンステート::D3Dレンダリング状態オブジェクト
	mCommonState = std::make_unique<DirectX::CommonStates>(device);

	// スプライトバッチ::デバッグ情報の表示に必要
	mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	mSpriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	// テクスチャの読み込み
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/gool.png",
		nullptr,
		mTexture.ReleaseAndGetAddressOf()
	);

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
	mEffect->SetFade(true);
	mEffect->Initialize(100.0f, DirectX::SimpleMath::Vector3(10.0f, 10.0f, 0.0f));
	mEffect->SetFade(false);
}


// 更新
// 戻り値	:次のシーン番号
GAME_SCENE ResultScene::Update(const DX::StepTimer& timer)
{
	// 警告よけ
	UNREFERENCED_PARAMETER(timer);

	mEffect->Update(timer);
	// キー入力情報を取得する
	auto keyState = DirectX::Keyboard::Get().GetState();

	// マウス入力情報を取得する
	auto mouseState = DirectX::Mouse::Get().GetState();

	if (keyState.Z || mouseState.leftButton)
	{
		return GAME_SCENE::TITLE;
	}

	return GAME_SCENE::NONE;
}

// 描画
void ResultScene::Draw()
{

	mSpriteBatch->Begin(SpriteSortMode_Deferred, mCommonState->NonPremultiplied());

	// スコアの描画変数
	SimpleMath::Vector2 pos(0,0);

	mSpriteBatch->Draw(mTexture.Get(), pos);

	// スコアを描画する
	wchar_t buf[32];
	swprintf_s(buf, 32, L"Score :%d", g_gameData.gScore);
	mSpriteFont->DrawString(mSpriteBatch.get(), buf, DirectX::XMFLOAT2(500, 180));

	mSpriteBatch->End();

	mEffect->Render();
}


// 終了処理
void ResultScene::Finalize()
{
}

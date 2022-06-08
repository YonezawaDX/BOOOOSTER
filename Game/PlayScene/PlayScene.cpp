/*
プレイシーン
*/
#include "pch.h"

#include "../GameMain.h"
#include "DeviceResources.h"

#include "Libraries/MyLibraries/GridFloor.h"
#include "Libraries/MyLibraries/Camera.h"
#include "Libraries/MyLibraries/FollowCamera.h"
#include "Libraries/MyLibraries/Circle.h"

#include "PlayScene.h"
#include "Stage.h"
#include "Player.h"
#include "Coin.h"
#include "Game/GameData.h"



// 名前空間の利用
using namespace DirectX;
using namespace DirectX::SimpleMath;

// コンストラクタ
PlayScene::PlayScene()
	: mFps(0)
	,mCoin{}
	,mCoinFlag{}
	,mCoinCount(0.0f)
	,mFadeEnd(false)
{
}

// デストラクタ
PlayScene::~PlayScene()
{
}

// 初期化
void PlayScene::Initialize()
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

	// プリミティブバッチ
	mPrimitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);

	// ベーシックエフェクト
	mBasicEffect = std::make_unique<DirectX::BasicEffect>(device);

	// 頂点カラーの有効化
	mBasicEffect->SetVertexColorEnabled(true);

	// インプットレイアウトの設定
	void const* shaderByteCode;
	size_t byteCodeLength;
	mBasicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		mInputLayout.ReleaseAndGetAddressOf()
	);


	// モデルデータの入っているフォルダの指定
	std::unique_ptr<EffectFactory> factory = std::make_unique<EffectFactory>(device);
	factory->SetDirectory(L"Resources/Models");

	// モデルのロード
	mModel = Model::CreateFromCMO(
		device,
		L"Resources/Models/BackGroundSphere.cmo",	// サイコロモデルを利用する例
		*factory
	);

	//背景用モデルの初期化
	mModelWorld = SimpleMath::Matrix::Identity;
	//ステージの生成
	mStage = std::make_unique<Stage>();

	// カメラの作成
	mCamera = std::make_unique<Camera>();
	mFollowCamera = std::make_unique<FollowCamera>();

	// プレイヤーの生成
	mPlayer = std::make_unique<Player>(mStage.get());
	mPlayer->Initialize();

	// マップ上にコインを生成する
	for (int i = 0; i < mStage->GetMapRow(); i++)
	{
		for (int j = 0; j < mStage->GetMapColumn(); j++)
		{
			// ブロックの描画位置をずらす
			DirectX::SimpleMath::Vector3 pos(j + 0.5f, -i - 0.5f, 0.0f);

			// 表示する3Dオブジェクトの種類
			if(mStage->GetMap()[i][j] == Stage::eMapType::Coin)
			{
				mCoin.push_back(std::make_unique<Coin>());
				if (!mCoin.empty())
				{
					mCoin[mCoin.size() - 1]->Initialize(pos);
					mCoinFlag.push_back(true);
				}
			}
		}
	}

	mEffect = std::make_unique<ShaderManager>();
	mEffect->Create();
	mEffect->SetFade(true);
	mEffect->Initialize(100.0f, Vector3(0.0f, 0.0f, 0.0f));
	mEffect->SetRenderState(
		mFollowCamera->GetTargetPosition() + Vector3(0.0f, 2.0f, 18.0f),
		mFollowCamera->GetEyePosition() + Vector3(0.0f, -15.0f, 100.0f),
		mFollowCamera->GetViewMatrix(),
		mFollowCamera->GetProjectionMatrix()
	);
	
}

// 更新
// 戻り値	:次のシーン番号
GAME_SCENE PlayScene::Update(const DX::StepTimer& timer)
{
	if (mFadeEnd == false)
	{
		mEffect->SetFade(false);
		mEffect->Update(timer);
	}

	if (mEffect->GetDarkScreen())
	{
		return GAME_SCENE::NONE;
	}
	// フレームレートの取得
	mFps = timer.GetFramesPerSecond();
	float time = float(timer.GetTotalSeconds());

	// カメラの設定
	// プレイヤーの向きによってカメラの位置を変更する
	mFollowCamera->SetPlayerDirection(mPlayer->GetPlayerDirection());
	mFollowCamera->SetRefTargetPosition(mPlayer->GetPlayerPos());
	mFollowCamera->SetRefEyePosition(mPlayer->GetPlayerPos() + mFollowCamera->TARGET_TO_EYE_VEC);
	// カメラの更新
	mFollowCamera->Update();

	// プレイヤーの更新
	mPlayer->Update(time,timer);
	
	// コインの更新(回転させる)
	for (unsigned int i = 0; i < mCoin.size(); i++)
	{
		if (mCoin[i] != nullptr)
		{
			mCoin[i]->Update(time);
		}
	}

	// コインとの判定
	bool no = true;
    for (unsigned int i = 0;i < mCoin.size(); i++)
    {
		if (mCoin[i] != nullptr)
		{
			// プレイヤーとの判定を取る
			no = mPlayer->GetPlayerPos().x + 0.5f < mCoin[i]->GetCoinPos().x - 0.5f ||
				 mPlayer->GetPlayerPos().y + 0.5f < mCoin[i]->GetCoinPos().y - 0.5f ||
				 mCoin[i]->GetCoinPos().x + 0.5f  < mPlayer->GetPlayerPos().x - 0.5f ||
				 mCoin[i]->GetCoinPos().y + 0.5f  < mPlayer->GetPlayerPos().y - 0.5f ;
		}
	    if (!no)
		{
			// プレイヤーと当たった場合そのコインを削除
			mCoin[i].reset(nullptr);
			mCoinFlag[i] = false;
			mCoinCount++;
	    }
	     
	}

	// ステージの更新
	mStage->Update(time);

	bool gool = true;
	gool = mPlayer->GetPlayerPos().x + 0.5f < mStage->GetGoolPos().x - 0.5f ||
		mPlayer->GetPlayerPos().y + 0.5f < mStage->GetGoolPos().y - 0.5f ||
		mStage->GetGoolPos().x + 0.5f < mPlayer->GetPlayerPos().x - 0.5f ||
		mStage->GetGoolPos().y + 0.5f < mPlayer->GetPlayerPos().y - 0.5f;
	// ゴール判定
	if (mPlayer->GetPlayerLife() < 0.0f || !gool)
	{
		mFadeEnd = true;
	}

	// フェードインの処理
	if (mFadeEnd)
	{
		mEffect->SetFade(true);
		mEffect->Update(timer);
		if (mEffect->GetDarkScreen())
		{
			return GAME_SCENE::RESULT;
		}
	}

	
	return GAME_SCENE::NONE;
}

// 描画
void PlayScene::Draw()
{
	// デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();


	// デバイスコンテキストの取得
	auto context = pDR->GetD3DDeviceContext();

	// 変換行列の宣言(ワールド、ビュー、射影)
	SimpleMath::Matrix world, view, projection;

	// ワールド行列の初期化(単位行列)
	world = SimpleMath::Matrix::Identity;

	// 射影行列の取得
	projection = mFollowCamera->GetProjectionMatrix();

	// ビュー行列の取得
	view = mFollowCamera->GetViewMatrix();

	// ステージの描画
	mStage->Draw(mCommonState.get(), mFollowCamera.get());

	// 背景の描画
	mModel->Draw(
		context,
		*mCommonState,
		mModelWorld,
		view,
		projection
	);

	// コインの描画
	int i = 0;
	for (auto& c : mCoin)
	{
		if (mCoinFlag[i])
		{
			c->Draw(mCommonState.get(), mFollowCamera.get());
		}
		i++;
	}

	// FPSの取得と表示文字列の準備
	wchar_t buf[32];
	swprintf_s(buf, 32, L"FPS = %d", mFps);

	// スプライトバッチによるスコアの表示
	mSpriteBatch->Begin();
	// スコアの表示--------------------------------------------------------------------
	swprintf_s(buf, 32, L"Score : %d", g_gameData.gScore);
	mSpriteFont->DrawString(mSpriteBatch.get(), buf, DirectX::XMFLOAT2(500, 10));
	g_gameData.gScore = static_cast<int>(mCoinCount * 100.0f);
	//-----------------------------------------------------------------------------------
	mSpriteBatch->End();

	// プレイヤーの描画
	mPlayer->Draw(mCommonState.get(),mFollowCamera.get());


	// 以下UIの描画-------------------------------------------------------------------------
	// ベーシックエフェクトの設定
	mBasicEffect->SetWorld(world);
	mBasicEffect->SetView(view);
	mBasicEffect->SetProjection(projection);
	mBasicEffect->Apply(context);

	// 入力レイアウトの設定：ポリゴンの頂点型によって入力レイアウトの中身が変わる
	context->IASetInputLayout(mInputLayout.Get());

	// ブレンドステートの設定：アルファブレンディングの描画方法の指定
	context->OMSetBlendState(mCommonState->NonPremultiplied(), nullptr, 0xffffffff);

	// 深度ステンシルステートの設定：深度ステンシルバッファの使い方の指定
	context->OMSetDepthStencilState(mCommonState->DepthDefault(), 0);

	// Circleオブジェクトのローカル変数の指定
	Circle circle1;
	DirectX::SimpleMath::Vector3 pos1(mFollowCamera->GetTargetPosition() + DirectX::SimpleMath::Vector3(-4.5f, 2.5f, 0.0f));
	DirectX::SimpleMath::Vector3 pos2(mFollowCamera->GetTargetPosition()+ DirectX::SimpleMath::Vector3(-4.5f,3.5f,0.0f));

	// プリミティブバッチによるポリゴンの描画
	mPrimitiveBatch->Begin();
	// パラメータ：プリミティブバッチ、中心座標、半径、円弧(0.0f-1.0f)、色、透過のアルファ値
	circle1.Draw(mPrimitiveBatch.get(), pos1, 0.3f, mPlayer->GetBoostTime()/100.0f, DirectX::Colors::Yellow, 0.5f);
	// パラメータ：プリミティブバッチ、中心座標、半径、円弧(0.0f-1.0f)、色、透過のアルファ値
	circle1.Draw(mPrimitiveBatch.get(), pos2, 0.3f, mPlayer->GetPlayerLife() / 100.0f, DirectX::Colors::Red, 0.5f);
	mPrimitiveBatch->End();

	mEffect->Render();
}


// 終了処理
void PlayScene::Finalize()
{
}

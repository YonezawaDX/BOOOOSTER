/*
コインクラス
*/
#include "pch.h"
#include "Coin.h"
#include "Model3D.h"
#include "Player.h"
#include "Stage.h"

// 定数を設定-------------------------------
const float Coin::MODEL_ROTATE_SPEED = 200.0f;
//------------------------------------------
// コンストラクタ
Coin::Coin()
	:mPos(DirectX::SimpleMath::Vector3::Zero)
	,mCoinWorld(DirectX::SimpleMath::Matrix::Identity)
{
}

// デストラクタ
Coin::~Coin()
{
}

// 初期化
void Coin::Initialize(DirectX::SimpleMath::Vector3 pos)
{
	// モデルを作る
	mCoinModel3D = std::make_unique<Model3D>();
	mCoinModel3D->Initialize(L"Resources/Models/CoinGame.cmo");

	// 座標を引数からもらう
	mPos = pos;

	//ワールド座標を設定する
	mCoinWorld = DirectX::SimpleMath::Matrix::CreateTranslation(mPos);
}

// 更新
void Coin::Update(float time)
{
	// コインを回転させる
	mCoinModel3D->SetModelRotationY(time * MODEL_ROTATE_SPEED);
}

// 描画
void Coin::Draw(DirectX::CommonStates* commonStates, FollowCamera* camera)
{
	// コインを描画する
	mCoinModel3D->Draw(commonStates, &mCoinWorld, camera);
}

// 終了処理
void Coin::Finalize()
{
}

/*
プレイシーン
*/
#pragma once

#include <CommonStates.h>
#include <Model.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <VertexTypes.h>

#include "../IScene.h"
#include "Game/GameMain.h"

#include "../ShaderManager.h"

// 前方宣言
class GridFloor;
class Camera;
class Stage;
class Player;
class FollowCamera;
class Coin;
class EffectManager;
class PlayScene : public IScene
{
private:

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> mCommonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

	// デバッグカメラ
	std::unique_ptr<Camera> mCamera;
	std::unique_ptr<FollowCamera> mFollowCamera;
	
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> mPrimitiveBatch;

	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> mBasicEffect;

	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
	
	
	
	// モデル
	std::unique_ptr<DirectX::Model> mModel;
	// ワールド座標
	DirectX::SimpleMath::Matrix mModelWorld;
	unsigned int mFps;

	// ステージ
	std::unique_ptr<Stage> mStage;

	// プレイヤー
	std::unique_ptr<Player> mPlayer;

	// コイン
	std::vector<std::unique_ptr<Coin>> mCoin;
	// コインの獲得用
	std::vector<bool> mCoinFlag;
	// 獲得したコインの量
	float mCoinCount;

	// Fadeの処理用
	std::unique_ptr<ShaderManager> mEffect;

	bool mFadeEnd;
	
public:
	// コンストラクタ
	PlayScene();

	// デストラクタ
	~PlayScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;
};
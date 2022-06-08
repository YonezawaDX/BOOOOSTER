/*
リザルトシーン
*/
#pragma once

#include <CommonStates.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "../IScene.h"
#include "Game/GameMain.h"
#include "Game/ShaderManager.h"

class ResultScene : public IScene
{
private:

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> mCommonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;

	// Fadeの処理用
	std::unique_ptr<ShaderManager> mEffect;

public:

	// コンストラクタ
	ResultScene();

	// デストラクタ
	~ResultScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;
};
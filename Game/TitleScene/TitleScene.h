/*
タイトルシーン
*/
#pragma once

#include <CommonStates.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "../IScene.h"
#include "Game/GameMain.h"

#include "Game/ShaderManager.h"

class TitleScene : public IScene
{
private:
	// 定数--------------------------------------------------
	static const int PUTH_Z_X;		// 文字のX座標
	static const int PUTH_Z_Y;		// 文字のY座標

	static const float TITLE_BACK_X;		// 背景のX座標
	static const float TITLE_BACK_Y;		// 背景のY座標

	static const float TITLE_X;		// title文字のX座標
	static const float TITLE_Y;		// title文字のY座標
	//-------------------------------------------------------
private:

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> mCommonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture2;

	// Fadeの処理用
	std::unique_ptr<ShaderManager> mEffect;

public:

	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;
};
//	エフェクト管理クラス	
//	EffectManager.h
#pragma once

#include "StepTimer.h"

#include <SimpleMath.h>
#include <list>
#include <WICTextureLoader.h>

#include "MyEffect.h"
#include "PlayScene/Player.h"

class EffectManager
{
public:
	// 定数-----------------------------------------
	static const float EFFECT_SPEED; // エフェクトの速度
	//---------------------------------------------
public:
	// エフェクトの生成
	void Create();

	//Create()で全部処理は長いので分割
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos);

	// 更新
	void Update(const DX::StepTimer& timer);

	// 描画
	void Render();

	// 描画位置の変更など
	void SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	
	// エフェクトの向きを変更する
	void SetEffectDirection(Player::ePlayerDirection playerDirection) { mEffectDirection = playerDirection; }

private:
	//バッチエフェクト
	std::unique_ptr<DirectX::AlphaTestEffect> mBatchEffect;

	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> mBatch;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;

	//エフェクトのポインタ
	//MyEffect* mEffect;
	std::list<std::unique_ptr<MyEffect>> mEffectList;

	//テクスチャデータ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mTexture;

	// エフェクトの向きを設定する
	Player::ePlayerDirection mEffectDirection;
};
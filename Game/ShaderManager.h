//-----------------------------------
//	エフェクト管理クラス	
//	ShaderManager.h
//-----------------------------------
#pragma once

#include "StepTimer.h"

#include <SimpleMath.h>
#include <list>
#include <WICTextureLoader.h>
#include "ShaderSample.h"

class ShaderManager
{
private:

	Microsoft::WRL::ComPtr<ID3D11Buffer>	mCBuffer;
	DirectX::SimpleMath::Matrix			mWorld;
	DirectX::SimpleMath::Matrix			mView;
	DirectX::SimpleMath::Matrix			mProj;
	DirectX::SimpleMath::Vector3		mCamera;
	DirectX::SimpleMath::Vector3		mTarget;

	//バッチエフェクト
	std::unique_ptr<DirectX::AlphaTestEffect> mBatchEffect;
	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> mBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> mStates;
	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture2;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> mGeometryShader;

	//エフェクトのポインタ
	std::list<std::unique_ptr<ShaderSample>>	mEffectList;
	std::vector<DirectX::VertexPositionColorTexture>  mVertex;

	// 秒数格納
	float mSecond;

	float mAlpaNum;

	//Fadeはインもしくはあうとか
	bool mFadeStart;

	// 画面が暗くなったか
	bool mDarkScreen;
public:

	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

	void Create();
	//Create()で全部処理は長いので分割
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos);
	void InitializeAverage(float life, DirectX::SimpleMath::Vector3 pos);
	void InitializeRandom(float life, DirectX::SimpleMath::Vector3 pos);
	void InitializeCone(float life, DirectX::SimpleMath::Vector3 pos);

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	void Update(DX::StepTimer timer);
	void Render();
	void ShaderDraw(int num, std::list<std::unique_ptr<ShaderSample>>::iterator ite);

	void SetRenderState(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	void Lost();

	void SetGravity(bool gravity);

	// Fadeがスタートされる
	void SetFade(bool fade) { mFadeStart = fade; }

	bool GetDarkScreen() { return mDarkScreen; }
};
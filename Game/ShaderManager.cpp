#include "pch.h"
#include "ShaderManager.h"

#include "DeviceResources.h" 
#include "StepTimer.h" 
#include <SimpleMath.h> 
#include <Effects.h> 
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include <CommonStates.h> 
#include <Keyboard.h>

#include "BinaryFile.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
const std::vector<D3D11_INPUT_ELEMENT_DESC> ShaderManager::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
//-----------------------------------------------------------------------------------
//	Create()
//	エフェクトを生成するついでにバッチ等必要なものを生成
//	IN	:	デバイス		DX::DeviceResources* deviceResources	
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//D3DDeviceを取得する
	auto device = pDR->GetD3DDevice();
	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		mInputLayout.GetAddressOf());

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, mVertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, mPixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, mGeometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	const wchar_t* name = L"Resources\\Textures\\Block.jpg";
	DirectX::CreateWICTextureFromFile(pDR->GetD3DDevice(), name, nullptr, mTexture.GetAddressOf());

	const wchar_t* name2 = L"Resources\\Textures\\ball.png";
	DirectX::CreateWICTextureFromFile(pDR->GetD3DDevice(), name2, nullptr, mTexture2.GetAddressOf());
	// プリミティブバッチの作成
	mBatch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	mStates = std::make_unique<CommonStates>(device);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	device->CreateBuffer(&bd, nullptr, &mCBuffer);

	mWorld = Matrix::Identity;
	//エフェクトを実体化し、リストに登録する。
	int count = 1;
	for (int i = 0; i < count; i++) {
		std::unique_ptr<ShaderSample> effect = std::make_unique<ShaderSample>();
		effect->Create();
		mEffectList.push_back(std::move(effect));
	}
	mFadeStart = false;
	//mDarkScreen = false;
}
//-----------------------------------------------------------------------------------
//	Initialize()
//	外部呼出し用のラッパー
//	IN	:	生存時間		float life
//			発生の中心		Vector3 pos
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::Initialize(float life, Vector3 pos)
{
	//InitializeAverage(life, pos);
	if (mFadeStart)
	{
		mAlpaNum = 0.0f;
		mDarkScreen = true;
	}
	else
	{
		mAlpaNum = 3.0f;
		mDarkScreen = false;
	}
}
//-----------------------------------------------------------------------------------
//	InitializeRandom()
//	ランダムな方向に飛ぶエフェクトを使うときの初期化
//	IN	:	生存時間		float life
//			発生の中心		Vector3 pos
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::InitializeRandom(float life, Vector3 pos)
{
	int range = 100;

	//life,pos,vel の値でm_effectを初期化する
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		Vector3 vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
		while (vel.Length() < 0.03f)
		{
			vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
		}

		(*ite)->Initialize(life, pos, vel);
	}

}
//-----------------------------------------------------------------------------------
//	InitializeAverage()
//	等間隔に配置して飛ぶエフェクトを使うときの初期化
//	IN	:	生存時間		float life
//			発生の中心		Vector3 pos
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::InitializeAverage(float life, Vector3 pos)
{
	const float RAD = XM_PI * 2.0f;
	int num = 0;
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		float size = (float)mEffectList.size();
		//Vector3 vel = Vector3(cos(RAD * num / size + XM_PI / 2.0f), sin(RAD * num / size + XM_PI / 2.0f), 0);
		
		int range = 100;

		Vector3 vel = Vector3(
			-0.5f,
			((rand() % (range * 2)) - range) / (float)range * 0.1f,
			((rand() % (range * 2)) - range) / (float)range * 0.1f
		);
		vel *= 1.0f;
		(*ite)->Initialize(life, pos, vel);
		num++;
	}
}
//-----------------------------------------------------------------------------------
//	InitializeCone()
//	円錐状に飛ぶエフェクトを使うときの初期化
//	IN	:	生存時間		float life
//			発生の中心		Vector3 pos
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::InitializeCone(float life, Vector3 pos)
{
	//エフェクトの中心軸
	Vector3 dir = Vector3(-3, 1, -5);

	//軸の値を正規化する
	dir.Normalize();

	//外積に使うためのカメラ位置（本当は外から受け渡せると良い）
	Vector3 camera = mCamera;


	//life,pos,vel の値でm_effectを初期化する
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		//軸とカメラから外積を求める
		Vector3 d = dir.Cross(camera);
		//外積を正規化する
		d.Normalize();
		//外積の値と中心軸を加算する
		d = dir + d * (float)sin(rand()) * 0.3f;
		//値が大きいので0.1倍
		d *= 0.1f;
		(*ite)->Initialize(life, pos, d);
	}
}
//-----------------------------------------------------------------------------------
//	Update()
//	更新処理
//	IN	:	タイマー		DX::StepTimer timer
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::Update(DX::StepTimer timer)
{
	float time = float(timer.GetTotalSeconds());
	mVertex.clear();
	//timerを渡してm_effectの更新処理を行う
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		(*ite)->Update(timer);

		//mWorld = Matrix::CreateScale(sin(time));
		
		VertexPositionColorTexture vertex;
		Vector3 pos = (*ite)->GetPosition();
		vertex = VertexPositionColorTexture(pos, Vector4::One, Vector2(0.0f, 0.0f));
		mVertex.push_back(vertex);
	}


	// キー入力情報を取得する
	auto keyState = DirectX::Keyboard::Get().GetState();
	
	if (mAlpaNum < 0.0f)
	{
		// くらい
		mAlpaNum = 0.0f;
		mDarkScreen = true;
	}
	else if (mAlpaNum > 3.0f)
	{
		// 明るい
		mAlpaNum = 3.0f;
		mDarkScreen = false;
	}

	if (mFadeStart == false)
	{
		mAlpaNum += 0.05f;
	}
	else
	{
		mAlpaNum -= 0.05f;
	}
}
//-----------------------------------------------------------------------------------
//	Render()
//	描画処理
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::Render()
{
	int i = 0;
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		this->ShaderDraw(i, ite);
	
		i++;
	}
}

// 描画の詳細設定
void ShaderManager::ShaderDraw(int num, std::list<std::unique_ptr<ShaderSample>>::iterator ite)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	auto context = pDR->GetD3DDeviceContext();

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = mView.Transpose();
	cbuff.matProj = mProj.Transpose();
	cbuff.matWorld = mWorld.Transpose();
	//cbuff.Diffuse = Vector4(1, 1, 1, 1);

	Vector3 pos = mVertex[num].position;

	// これを改造マン--------------------------------------------------
	cbuff.Diffuse = Vector4(pos.x, pos.y, pos.z, mAlpaNum);
	//-----------------------------------------------------------------


	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(mCBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { mCBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);

	//画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { mStates->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//半透明描画指定
	ID3D11BlendState* blendstate = mStates->NonPremultiplied();


	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(mStates->DepthRead(), 0);
	// カリングは左周り
	context->RSSetState(mStates->CullNone());

	//シェーダをセットする
	context->VSSetShader(mVertexShader.Get(), nullptr, 0);
	context->PSSetShader(mPixelShader.Get(), nullptr, 0);
	context->GSSetShader(mGeometryShader.Get(), nullptr, 0);
	//ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, mTexture.GetAddressOf());
	context->PSSetShaderResources(1, 1, mTexture2.GetAddressOf());

	//インプットレイアウトの登録
	context->IASetInputLayout(mInputLayout.Get());

	// 点ポリゴンを描画
	mBatch->Begin();
	mBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &mVertex[num], 1);
	mBatch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
}



//-----------------------------------------------------------------------------------
//	SetRenderState()
//	カメラ、ビュー、射影の反映
//	IN	:	カメラ		DirectX::SimpleMath::Vector3 camera
//			ビュー行列	DirectX::SimpleMath::Matrix view
//			射影行列	DirectX::SimpleMath::Matrix proj
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::SetRenderState(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	mTarget = target;
	mCamera = camera;
	mView = view;
	mProj = proj;
	mWorld = Matrix::CreateBillboard(mTarget, mCamera, Vector3::UnitY);
}
//-----------------------------------------------------------------------------------
//	SetGravity()
//	重力フラグ
//	IN	:	重力フラグ		bool gravity
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::SetGravity(bool gravity)
{
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		(*ite)->SetGravity(gravity);
	}
}

//-----------------------------------------------------------------------------------
//	Lost()
//	破棄
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::Lost()
{
	mEffectList.clear();
	mInputLayout.Reset();
	mTexture.Reset();
}
/*
Modelクラス
*/
#include "pch.h"
#include "Model3D.h"

#include <Effects.h>
#include "DeviceResources.h"
#include "Libraries/MyLibraries/FollowCamera.h"

// コンストラクタ
Model3D::Model3D()
	:mModel()
	,mRotateY(0.0f)
	,mRotateZ(0.0f)
{
}


// 初期化
void Model3D::Initialize(const wchar_t* fileName)
{
	// デバイスの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();

	// モデルデータの入っているフォルダの指定
	std::unique_ptr<DirectX::EffectFactory> factory = std::make_unique<DirectX::EffectFactory>(device);
	factory->SetDirectory(L"Resources/Models");

	// 3Dモデルのロード
	mModel = DirectX::Model::CreateFromCMO(device, fileName, *factory);
}

// 描画
void Model3D::Draw(DirectX::CommonStates* commonStates, DirectX::SimpleMath::Matrix* world, FollowCamera* camera)
{
	//デバイスコンテキストの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();

	// 射影行列の取得
	DirectX::SimpleMath::Matrix projection = camera->GetProjectionMatrix();

	// ビュー行列の取得
	DirectX::SimpleMath::Matrix view = camera->GetViewMatrix();

	// モデルの回転も行う
	// モデルの描画
	mModel->Draw(
		context,
		*commonStates,
		DirectX::SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(mRotateZ))
		* DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(mRotateY))*
		*world ,
		view,
		projection);
}

// 終了処理
void Model3D::Finalize()
{
}

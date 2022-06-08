/*
Model�N���X
*/
#include "pch.h"
#include "Model3D.h"

#include <Effects.h>
#include "DeviceResources.h"
#include "Libraries/MyLibraries/FollowCamera.h"

// �R���X�g���N�^
Model3D::Model3D()
	:mModel()
	,mRotateY(0.0f)
	,mRotateZ(0.0f)
{
}


// ������
void Model3D::Initialize(const wchar_t* fileName)
{
	// �f�o�C�X�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();

	// ���f���f�[�^�̓����Ă���t�H���_�̎w��
	std::unique_ptr<DirectX::EffectFactory> factory = std::make_unique<DirectX::EffectFactory>(device);
	factory->SetDirectory(L"Resources/Models");

	// 3D���f���̃��[�h
	mModel = DirectX::Model::CreateFromCMO(device, fileName, *factory);
}

// �`��
void Model3D::Draw(DirectX::CommonStates* commonStates, DirectX::SimpleMath::Matrix* world, FollowCamera* camera)
{
	//�f�o�C�X�R���e�L�X�g�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();

	// �ˉe�s��̎擾
	DirectX::SimpleMath::Matrix projection = camera->GetProjectionMatrix();

	// �r���[�s��̎擾
	DirectX::SimpleMath::Matrix view = camera->GetViewMatrix();

	// ���f���̉�]���s��
	// ���f���̕`��
	mModel->Draw(
		context,
		*commonStates,
		DirectX::SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(mRotateZ))
		* DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(mRotateY))*
		*world ,
		view,
		projection);
}

// �I������
void Model3D::Finalize()
{
}

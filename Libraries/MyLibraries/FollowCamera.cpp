//
// Camera.cpp
//
#include "pch.h"
#include "FollowCamera.h"

#include "DeviceResources.h"
#include <Keyboard.h>

// �萔-----------------------------------------------------------------
const DirectX::SimpleMath::Vector3 FollowCamera::TARGET_TO_EYE_VEC = DirectX::SimpleMath::Vector3(0.0f, 2.0f, 10.0f);

const float FollowCamera::POINT_OF_VIEW_WORK = 1.5f;		// �v���C���[���n�ʂɂ���Ƃ��̎��_
const float FollowCamera::POINT_OF_VIEW_BOOST = 7.0f;		// �v���C���[�����ł���Ƃ��̎��_
//-------------------------------------------------------------------------------------------------

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �R���X�g���N�^
FollowCamera::FollowCamera()
	: mView(DirectX::SimpleMath::Matrix::Identity)
	, mEye(0.0f, 2.0f, 20.0f)
	, mTarget(0.0f, 0.0f, 0.0f)
	, mUp(0.0f, 1.0f, 0.0f)
	, mCameraRot(0.0f)
	,mPlayerDir(0.0f)
{
	CalculateViewMatrix();
	CalculateProjectionMatrix();
}

// �f�X�g���N�^
FollowCamera::~FollowCamera()
{
}

// �X�V
void FollowCamera::Update()
{
	SimpleMath::Vector3 dir = SimpleMath::Vector3(mPlayerDir, 2.0f, 10.0f);
	mEye += (mRefEye - mEye + dir) * 0.035f;

	mTarget += (mRefTarget - mTarget + dir) * 0.035f;

	CalculateViewMatrix();
}

// �v���C���[�̌���
void FollowCamera::SetPlayerDirection(int playerDirection)
{
	if (playerDirection == static_cast<int>(ePlayerDirection::LEFT))
	{
		mPlayerDir = -POINT_OF_VIEW_WORK;
	}
	else if(playerDirection == static_cast<int>(ePlayerDirection::RIGHT))
	{
		mPlayerDir = POINT_OF_VIEW_WORK;
	}
	// �u�[�X�g��
	else if (playerDirection == static_cast<int>(ePlayerDirection::BOOST_LEFT))
	{
		mPlayerDir = -POINT_OF_VIEW_BOOST;
	}
	else if (playerDirection == static_cast<int>(ePlayerDirection::BOOST_RIGHT))
	{
		mPlayerDir = POINT_OF_VIEW_BOOST;
	}
}

// �r���[�s��̎Z�o
void FollowCamera::CalculateViewMatrix()
{
	mView = DirectX::SimpleMath::Matrix::CreateLookAt(mEye , mTarget, mUp);
}

// �v���W�F�N�V�����s��̎Z�o
void FollowCamera::CalculateProjectionMatrix()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// �E�B���h�E�T�C�Y�̎擾
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	constexpr float fieldOfView = XMConvertToRadians(45.0f);    // ��p
	float aspectRatio = width / height;							// ��ʏc����
	float nearPlane = 1.0f;                                     // �J���������ԋ߂����e��
	float farPlane = 500.0f;                                    // �J���������ԉ������e��
	mProjection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
}

//
// Camera.cpp
//
#include "pch.h"
#include "FollowCamera.h"

#include "DeviceResources.h"
#include <Keyboard.h>

// 定数-----------------------------------------------------------------
const DirectX::SimpleMath::Vector3 FollowCamera::TARGET_TO_EYE_VEC = DirectX::SimpleMath::Vector3(0.0f, 2.0f, 10.0f);

const float FollowCamera::POINT_OF_VIEW_WORK = 1.5f;		// プレイヤーが地面にいるときの視点
const float FollowCamera::POINT_OF_VIEW_BOOST = 7.0f;		// プレイヤーが飛んでいるときの視点
//-------------------------------------------------------------------------------------------------

using namespace DirectX;
using namespace DirectX::SimpleMath;

// コンストラクタ
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

// デストラクタ
FollowCamera::~FollowCamera()
{
}

// 更新
void FollowCamera::Update()
{
	SimpleMath::Vector3 dir = SimpleMath::Vector3(mPlayerDir, 2.0f, 10.0f);
	mEye += (mRefEye - mEye + dir) * 0.035f;

	mTarget += (mRefTarget - mTarget + dir) * 0.035f;

	CalculateViewMatrix();
}

// プレイヤーの向き
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
	// ブースト中
	else if (playerDirection == static_cast<int>(ePlayerDirection::BOOST_LEFT))
	{
		mPlayerDir = -POINT_OF_VIEW_BOOST;
	}
	else if (playerDirection == static_cast<int>(ePlayerDirection::BOOST_RIGHT))
	{
		mPlayerDir = POINT_OF_VIEW_BOOST;
	}
}

// ビュー行列の算出
void FollowCamera::CalculateViewMatrix()
{
	mView = DirectX::SimpleMath::Matrix::CreateLookAt(mEye , mTarget, mUp);
}

// プロジェクション行列の算出
void FollowCamera::CalculateProjectionMatrix()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	constexpr float fieldOfView = XMConvertToRadians(45.0f);    // 画角
	float aspectRatio = width / height;							// 画面縦横比
	float nearPlane = 1.0f;                                     // カメラから一番近い投影面
	float farPlane = 500.0f;                                    // カメラから一番遠い投影面
	mProjection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
}

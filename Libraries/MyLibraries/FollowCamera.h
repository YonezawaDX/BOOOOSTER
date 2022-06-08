//
// Camera.h
//
#pragma once

#include <SimpleMath.h>

class FollowCamera
{
public:
	static const float DEFAULT_CAMERA_DISTANCE;	// カメラの距離
	static const DirectX::SimpleMath::Vector3 TARGET_TO_EYE_VEC; // 視点と注視点

	static const float POINT_OF_VIEW_WORK;		// プレイヤーが地面にいるときの視点
	static const float POINT_OF_VIEW_BOOST;		// プレイヤーが飛んでいるときの視点

	enum class ePlayerDirection : int
	{
		LEFT = 1,
		RIGHT,
		BOOST_LEFT,
		BOOST_RIGHT
	};

private:

	// ビュー行列
	DirectX::SimpleMath::Matrix mView;

	// プロジェクション行列
	DirectX::SimpleMath::Matrix mProjection;

	// カメラ座標
	DirectX::SimpleMath::Vector3 mEye;

	// 注視点
	DirectX::SimpleMath::Vector3 mTarget;

	// 上向きベクトル
	DirectX::SimpleMath::Vector3 mUp;

	// 参照視点
	DirectX::SimpleMath::Vector3 mRefEye;

	// 参照注視点
	DirectX::SimpleMath::Vector3 mRefTarget;

	//回転値
	float mCameraRot;

	float mPlayerDir;
public:
	// コンストラクタ
	FollowCamera();

	// デストラクタ
	~FollowCamera();

	// 更新
	void Update();

	// ビュー行列のアクセサ
	void SetViewMatrix(DirectX::SimpleMath::Matrix view) { mView = view; }
	DirectX::SimpleMath::Matrix GetViewMatrix() const { return mView; }

	// プロジェクション行列のアクセサ
	void SetProjectionMatrix(DirectX::SimpleMath::Matrix projection) { mProjection = projection; }
	DirectX::SimpleMath::Matrix GetProjectionMatrix() const { return mProjection; }

	// カメラ座標のアクセサ
	void SetEyePosition(DirectX::SimpleMath::Vector3 eye) { mEye = eye; }
	DirectX::SimpleMath::Vector3 GetEyePosition() const { return mEye; }

	// 注視点座標のアクセサ
	void SetTargetPosition(DirectX::SimpleMath::Vector3 target) { mTarget = target; }
	DirectX::SimpleMath::Vector3 GetTargetPosition() const { return mTarget; }

	// 上向きベクトルのアクセサ
	void SetUpVector(DirectX::SimpleMath::Vector3 up) { mUp = up; }
	DirectX::SimpleMath::Vector3 GetUpVector() const { return mUp; }

	// 参照視点点のアクセサ
	void SetRefEyePosition(DirectX::SimpleMath::Vector3 refeye) { mRefEye = refeye; }
	void SetRefTargetPosition(DirectX::SimpleMath::Vector3 reftarget) { mRefTarget = reftarget; }

	// カメラのゲッター
	float GetCameraRot() const { return mCameraRot; }


	// プレイヤーの向いてる方向を設定する
	void SetPlayerDirection(int playerDirection);

protected:

	// ビュー行列の算出
	virtual void CalculateViewMatrix();

	// プロジェクション行列の算出
	virtual void CalculateProjectionMatrix();
};

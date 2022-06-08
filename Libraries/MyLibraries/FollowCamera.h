//
// Camera.h
//
#pragma once

#include <SimpleMath.h>

class FollowCamera
{
public:
	static const float DEFAULT_CAMERA_DISTANCE;	// �J�����̋���
	static const DirectX::SimpleMath::Vector3 TARGET_TO_EYE_VEC; // ���_�ƒ����_

	static const float POINT_OF_VIEW_WORK;		// �v���C���[���n�ʂɂ���Ƃ��̎��_
	static const float POINT_OF_VIEW_BOOST;		// �v���C���[�����ł���Ƃ��̎��_

	enum class ePlayerDirection : int
	{
		LEFT = 1,
		RIGHT,
		BOOST_LEFT,
		BOOST_RIGHT
	};

private:

	// �r���[�s��
	DirectX::SimpleMath::Matrix mView;

	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix mProjection;

	// �J�������W
	DirectX::SimpleMath::Vector3 mEye;

	// �����_
	DirectX::SimpleMath::Vector3 mTarget;

	// ������x�N�g��
	DirectX::SimpleMath::Vector3 mUp;

	// �Q�Ǝ��_
	DirectX::SimpleMath::Vector3 mRefEye;

	// �Q�ƒ����_
	DirectX::SimpleMath::Vector3 mRefTarget;

	//��]�l
	float mCameraRot;

	float mPlayerDir;
public:
	// �R���X�g���N�^
	FollowCamera();

	// �f�X�g���N�^
	~FollowCamera();

	// �X�V
	void Update();

	// �r���[�s��̃A�N�Z�T
	void SetViewMatrix(DirectX::SimpleMath::Matrix view) { mView = view; }
	DirectX::SimpleMath::Matrix GetViewMatrix() const { return mView; }

	// �v���W�F�N�V�����s��̃A�N�Z�T
	void SetProjectionMatrix(DirectX::SimpleMath::Matrix projection) { mProjection = projection; }
	DirectX::SimpleMath::Matrix GetProjectionMatrix() const { return mProjection; }

	// �J�������W�̃A�N�Z�T
	void SetEyePosition(DirectX::SimpleMath::Vector3 eye) { mEye = eye; }
	DirectX::SimpleMath::Vector3 GetEyePosition() const { return mEye; }

	// �����_���W�̃A�N�Z�T
	void SetTargetPosition(DirectX::SimpleMath::Vector3 target) { mTarget = target; }
	DirectX::SimpleMath::Vector3 GetTargetPosition() const { return mTarget; }

	// ������x�N�g���̃A�N�Z�T
	void SetUpVector(DirectX::SimpleMath::Vector3 up) { mUp = up; }
	DirectX::SimpleMath::Vector3 GetUpVector() const { return mUp; }

	// �Q�Ǝ��_�_�̃A�N�Z�T
	void SetRefEyePosition(DirectX::SimpleMath::Vector3 refeye) { mRefEye = refeye; }
	void SetRefTargetPosition(DirectX::SimpleMath::Vector3 reftarget) { mRefTarget = reftarget; }

	// �J�����̃Q�b�^�[
	float GetCameraRot() const { return mCameraRot; }


	// �v���C���[�̌����Ă������ݒ肷��
	void SetPlayerDirection(int playerDirection);

protected:

	// �r���[�s��̎Z�o
	virtual void CalculateViewMatrix();

	// �v���W�F�N�V�����s��̎Z�o
	virtual void CalculateProjectionMatrix();
};

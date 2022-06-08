/*
Model�N���X
*/
#pragma once
#include <SimpleMath.h>
#include "StepTimer.h"
#include <CommonStates.h>
#include <Model.h>

// �O���錾
class FollowCamera;

class Model3D
{
public:
	// �֐�------------------------------------
	// �R���X�g���N�^
	Model3D();
	// �f�X�g���N�^
	~Model3D() = default;

	// ������
	void Initialize(const wchar_t* fileName);

	// �`��
	void Draw(
		DirectX::CommonStates* commonStates,
		DirectX::SimpleMath::Matrix* world,
		FollowCamera* camera
	);

	// �I������
	void Finalize();

	// ���f������]������
	void SetModelRotationY(float rotate) { mRotateY = rotate; }
	void SetModelRotationZ(float rotate) { mRotateZ = rotate; }

private:
	// ���f��
	std::unique_ptr<DirectX::Model> mModel;

	// ��]���̕ϐ�YZ
	float mRotateY;
	float mRotateZ;

};
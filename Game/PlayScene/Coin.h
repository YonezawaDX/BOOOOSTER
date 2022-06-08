/*
�R�C���N���X
*/
#pragma once
#include <SimpleMath.h>
#include <CommonStates.h>

class FollowCamera;
class Stage;
class Player;
class Model3D;
class Coin
{
public:
	// �萔�p-------------------------------------
	static const float MODEL_ROTATE_SPEED;
public:
	// �֐�----------------------------------------
	// �R���X�g���N�^
	Coin();
	//�f�X�g���N�^
	~Coin();

	// ������
	void Initialize(DirectX::SimpleMath::Vector3 pos);
	// �X�V
	void Update(float time);
	// �`��
	void Draw(DirectX::CommonStates* commonStates, FollowCamera* camera);
	// �I������
	void Finalize();

	// �R�C���̍��W��n��
	DirectX::SimpleMath::Vector3 GetCoinPos()const { return mPos; }
private:
	// �ϐ��p---------------------------------------------
	// �R�C�����̂̏����p
	// �R�C���̍��W
	DirectX::SimpleMath::Vector3 mPos;
	// �R�C���̃��f��
	std::unique_ptr<Model3D> mCoinModel3D;
	// �R�C���̃��[���h���W
	DirectX::SimpleMath::Matrix mCoinWorld;

};

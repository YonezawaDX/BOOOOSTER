/*
Stage�N���X
*/
#pragma once
#include <vector>
#include <CommonStates.h>
#include <SimpleMath.h>

class FollowCamera;
class Model3D;

class Stage
{
public:
	// �萔-----------------------------------------------------------------
	static const float CHIP_SIZE;			// �u���b�N�̃T�C�Y

	static const int SPIKE_INTERVAL_TIME;	// �������̔����Ă��鑬��
	
	static const float SPIKE_INVERSION;		// ���𔽓]������
	static const float SPIKE_NINTY;			// ����90�x�Ȃ���
	//--------------------------------------------------------------------------
	// �}�b�v�`�b�v�̎��
	enum class eMapType : int
	{
		Coin = -2,
		None = -1,
		Floor,		// 0
		Wall,		// 1
		Spike,		// 2
		Spike_Top,	// 3
		Spike_Left,	// 4
		Spike_Right,// 5
		Gool,		// 6
		Spike_Death,// 7
		MapNum		// 8	
	};

private:


	// �f�[�^�����o
	int mRowNum;								// �s��
	int mColumnNum;								// ��
	std::vector<std::vector<eMapType>> mMap;	// vector��2�����z��
	std::unique_ptr<Model3D> mModel3D[static_cast<int>(eMapType::MapNum)];	// 3D���f���̔z��

	// �R�C���̍��W��n��
	std::vector<DirectX::SimpleMath::Matrix> mCoinWorid;

	// �}�b�v�ύX�p�^�C�}�[
	int mTimer;

	// �}�b�v��ϊ�����
	int mChangeColumn;

	// �S�[���̍��W
	DirectX::SimpleMath::Vector3 mGoolPos;
public:
	// �R���X�g���N�^�A�f�X�g���N�^
	Stage();
	~Stage();

	// �s�����擾����
	int GetMapRow() const { return mRowNum; }

	// �񐔂��擾����
	int GetMapColumn() const { return mColumnNum; }

	// �}�b�v�z��̎Q�Ƃ��擾����
	std::vector<std::vector<eMapType>>& GetMap() { return mMap; }

	// �}�b�v��ύX���邽�߂̕ϊ���i
	void Update(float time);

	// �`��
	void Draw(DirectX::CommonStates* commonStates, FollowCamera* camera);

	// �}�b�v�`�b�v�̒l���擾����֐�
	int GetChipParam(float posX, float posY);

	// �V�����}�b�v���Z�b�g����
	void SetMapDestruction(std::vector<std::vector<eMapType>> map) { mMap = map; }

	// �S�[���̍��W���Q�b�g����
	DirectX::SimpleMath::Vector3 GetGoolPos() const { return mGoolPos; }

private:
	// �}�b�v�f�[�^��ǂݍ���
	bool LoadCSV(const wchar_t* mapFile);

	// �}�b�v���f����Load
	void LoadModel3D();
};
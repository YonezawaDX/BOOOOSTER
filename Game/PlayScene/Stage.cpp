/*
Stage�N���X
*/
#include "pch.h"
#include "Stage.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <Effects.h>
#include <SimpleMath.h>
#include "DeviceResources.h"
#include "Libraries/MyLibraries/FollowCamera.h"
#include "Model3D.h"
#include <Keyboard.h>

// �萔---------------------------------------------------------------
const int Stage::SPIKE_INTERVAL_TIME = 50;	// ���̔����Ă��鎞��

const float Stage::SPIKE_INVERSION = 180.0f;		// ���𔽓]������
const float Stage::SPIKE_NINTY = 90.0f;			// ����90�x�Ȃ���
//--------------------------------------------------------------------

// �R���X�g���N�^
Stage::Stage()
	:mColumnNum(0)
	,mRowNum(0)
	,mTimer(0)
	,mChangeColumn(0)
	,mGoolPos(DirectX::SimpleMath::Vector3::Zero)
{
	this->LoadCSV(L"Resources/CSV/Map.csv");
	this->LoadModel3D();
}

// �f�X�g���N�^
Stage::~Stage()
{
	// �}�b�v������
	mMap.clear();
}

// �X�V
void Stage::Update(float time)
{
	// �x���悯
	UNREFERENCED_PARAMETER(time);

	// �����Ă��鞙
	if (mTimer > SPIKE_INTERVAL_TIME)
		mTimer = 0;
	mTimer++;
}

// �}�b�v�̕`��
void Stage::Draw(DirectX::CommonStates* commonStates, FollowCamera* camera)
{

	// �Ԃ����𔗂点��
	if (mTimer == SPIKE_INTERVAL_TIME)
	{
		for (int i = 0; i < mRowNum; i++)
		{
			// ���������}�b�v�̒[�ɐ���
			mMap[i][mChangeColumn] = eMapType::Spike_Death;
			if (mChangeColumn > 0)
			{
				// ��O�̃u���b�N������
				mMap[i][mChangeColumn - 1] = eMapType::Floor;
			}
		
		}
		// �ЂƂ��ɂ��炷
		mChangeColumn++;
	}

	for (int i = 0; i < mRowNum; i++)
	{
		for (int j = 0; j < mColumnNum; j++)
		{	
			// �u���b�N�̕`��ʒu�����炷
			DirectX::SimpleMath::Vector3 mapPos = DirectX::SimpleMath::Vector3(j + 0.5f, -i - 0.5f, 0.0f);
			DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(mapPos);
			
			// �\������3D�I�u�W�F�N�g�̎��
			switch (mMap[i][j])
			{
				// �Ȃɂ��Ȃ�
				case eMapType::Floor:
					break;
				// ��
				case eMapType::Wall:
					mModel3D[static_cast<int>(eMapType::Wall)]->Draw(commonStates, &world, camera);
					break;
				// ��
				case eMapType::Spike:
					mModel3D[static_cast<int>(eMapType::Spike)]->Draw(commonStates, &world, camera);
					break;				 
				// �R�C��
				case eMapType::Coin:
					break;
				
				// ��̞�
				case eMapType::Spike_Top:
					mModel3D[static_cast<int>(eMapType::Spike_Top)]->SetModelRotationZ(SPIKE_INVERSION);
					mModel3D[static_cast<int>(eMapType::Spike_Top)]->Draw(commonStates, &world, camera);
					break;
				// ���̞�
				case eMapType::Spike_Left:
					mModel3D[static_cast<int>(eMapType::Spike_Left)]->SetModelRotationZ(-SPIKE_NINTY);
					mModel3D[static_cast<int>(eMapType::Spike_Left)]->Draw(commonStates, &world, camera);
					break;
				// �E�̞�
				case eMapType::Spike_Right:
					mModel3D[static_cast<int>(eMapType::Spike_Right)]->SetModelRotationZ(SPIKE_NINTY);
					mModel3D[static_cast<int>(eMapType::Spike_Right)]->Draw(commonStates, &world, camera);
					break;
				
				// �S�[��
				case eMapType::Gool:
					mModel3D[static_cast<int>(eMapType::Gool)]->Draw(commonStates, &world, camera);
					mGoolPos = mapPos;
					break;
				
				// ������ 
				case eMapType::Spike_Death:
					mModel3D[static_cast<int>(eMapType::Spike_Death)]->SetModelRotationZ(-SPIKE_NINTY);
					mModel3D[static_cast<int>(eMapType::Spike_Death)]->Draw(commonStates, &world, camera);
					break;
				default:
					assert(!"�}�b�v�`�b�v�̕�������������܂���");
					break;
			}
		}
	}
}


// �}�b�v�`�b�v�̒l���擾����֐�
int Stage::GetChipParam(float posX, float posY)
{
	// �����ϊ��p�ϐ�
	int x, y;

	// �����l�֕ϊ�
	x = static_cast<int>(posX);
	y = static_cast<int>(posY);

	// �}�b�v����͂ݏo�Ă����� 0 ��Ԃ�
	//To do �������ق����ǂ������i�J�����͈̔͂Ȃ炠��B�}�b�v�̑傫���Ƃ��Ȃ��j
	if (x >= mColumnNum || y >= mRowNum || x < 0 || y < 0) return static_cast<int>(Stage::eMapType::None);

	// �w��̍��W�ɊY������}�b�v�̏���Ԃ�
	return static_cast<int>(mMap[y][x]);
}

// CSV�t�@�C������̃}�b�v�f�[�^�̓ǂݍ���
bool Stage::LoadCSV(const wchar_t* mapFile)
{
	// ���[�J���ϐ�
	std::ifstream ifs;		// ���̓t�@�C���X�g���[��
	std::istringstream iss;	// ���͕�����X�g���[��
	std::string line;		// 1�s���̕�����
	std::string data;		// (�J���}�ŋ�؂�ꂽ)1�f�[�^���̕�����
	
	// �t�@�C�����J��
	ifs.open(mapFile);

	// �t�@�C�����J���Ȃ���΁A���s
	if (ifs.fail()) { return false; }

	// �s����ǂݍ���
	std::getline(ifs, line);
	mRowNum = std::stoi(line);

	// �񐔂�ǂݍ���
	std::getline(ifs, line);
	mColumnNum = std::stoi(line);

	// 2�����z��̃T�C�Y���f�[�^�̑傫���ɍ��킹�Ē�������
	mMap.resize(mRowNum, std::vector<eMapType>(mColumnNum));

	// CSV����}�b�v�f�[�^��ǂݍ���
	for (int i = 0; i < mRowNum; i++)
	{
		// ��s���̕������ǂݍ���
		std::getline(ifs, line);
		// iss�̏�Ԃ��N���A -> ���Ȃ��ƃn�}��
		iss.clear(std::istringstream::goodbit);
		// �����񂩂當����X�g���[���ɕϊ�
		iss.str(line);

		// �J���}��؂�ŕ������int�^�ɕϊ����A�z��Ɋi�[����
		for (int j = 0; j < mColumnNum; j++)
		{
			std::getline(iss, data, ',');
			mMap[i][j] = static_cast<eMapType>(std::stoi(data));
		}
	}

	// �t�@�C�������
	ifs.close();

	// �t�@�C���̓ǂݍ��ݐ���
	return true;
}

// �}�b�v�̃��f����ǂݍ���
void Stage::LoadModel3D()
{
	// �f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();

	// ���f���f�[�^�̓����Ă���t�H���_�̎w��
	std::unique_ptr<DirectX::EffectFactory> factory = std::make_unique<DirectX::EffectFactory>(device);
	factory->SetDirectory(L"Resources/Models");

	//3D���f������
	for (unsigned int i = 0; i < static_cast<int>(eMapType::MapNum); i++)
	{

		mModel3D[i] = std::make_unique<Model3D>();
	}
	mModel3D[0]->Initialize(L"Resources/Models/Floor.cmo");
	mModel3D[1]->Initialize(L"Resources/Models/Wall.cmo");
	mModel3D[2]->Initialize(L"Resources/Models/spike.cmo");
	mModel3D[3]->Initialize(L"Resources/Models/spike.cmo");
	mModel3D[4]->Initialize(L"Resources/Models/spike.cmo");
	mModel3D[5]->Initialize(L"Resources/Models/spike.cmo");
	mModel3D[6]->Initialize(L"Resources/Models/Gool3.cmo");
	mModel3D[7]->Initialize(L"Resources/Models/spikeDeath.cmo");
}

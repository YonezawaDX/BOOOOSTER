#include "pch.h"
#include "ShaderSample.h"
#include "BinaryFile.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

using namespace DX;
using namespace DirectX;
using namespace DirectX::SimpleMath;

ShaderSample::ShaderSample()
	: mWait(0.0f)
	, mStartLife(0.0f)
	, mLife(0.0f)
	, mIsGravity(false)
{
}
ShaderSample::~ShaderSample()
{
	Lost();
}
//-----------------------------------------------------------------------------------
//	Create()
//	�V�F�[�_���𐶐�
//	IN	:	�f�o�C�X			DX::DeviceResources* deviceResources
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderSample::Create()
{
}

void ShaderSample::Initialize(float life, Vector3 pos, Vector3 velocity)
{
	mStartPosition = pos;
	mStartVelocity = velocity;
	mStartLife = life;

	//int seed = 5;
	//mWait = ((rand() % seed) / (float)seed) * mStartLife ;

	Reset();
	mGravity = Vector3(0, 0.005f, 0);
}

void ShaderSample::Reset()
{
	mPosition = mStartPosition;
	mVelocity = mStartVelocity;
	mLife = mStartLife;
}
//-----------------------------------------------------------------------------------
//	Update()
//	�X�V����
//	IN	:	�^�C�}�[		DX::StepTimer timer
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderSample::Update(StepTimer timer)
{
	//�����Ɏg���b���v(1�b��1.0f)���擾����B
	float time = float(mTimer.GetElapsedSeconds());
	mTimer = timer;

	//float rot_radian = mTimer.GetTotalSeconds();
	//mPosition = Vector3(cos(rot_radian), 0, sin(rot_radian));
	
	mVelocity -= mGravity;
	mPosition += mVelocity;

	Vector3 len = mPosition - mStartPosition;

	//if (len.Length() > 30.0f)
	//{
	//	mVelocity = Vector3(0.1f, 0.0f, 0.0f);
	//}
}
//-----------------------------------------------------------------------------------
//	Render()
//	�`�揈��(���C���ȊO�̊O���v���j
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderSample::Render()
{
}


void ShaderSample::Lost()
{
}

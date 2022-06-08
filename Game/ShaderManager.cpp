#include "pch.h"
#include "ShaderManager.h"

#include "DeviceResources.h" 
#include "StepTimer.h" 
#include <SimpleMath.h> 
#include <Effects.h> 
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include <CommonStates.h> 
#include <Keyboard.h>

#include "BinaryFile.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
const std::vector<D3D11_INPUT_ELEMENT_DESC> ShaderManager::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
//-----------------------------------------------------------------------------------
//	Create()
//	�G�t�F�N�g�𐶐�������łɃo�b�`���K�v�Ȃ��̂𐶐�
//	IN	:	�f�o�C�X		DX::DeviceResources* deviceResources	
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//D3DDevice���擾����
	auto device = pDR->GetD3DDevice();
	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		mInputLayout.GetAddressOf());

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, mVertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, mPixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, mGeometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	const wchar_t* name = L"Resources\\Textures\\Block.jpg";
	DirectX::CreateWICTextureFromFile(pDR->GetD3DDevice(), name, nullptr, mTexture.GetAddressOf());

	const wchar_t* name2 = L"Resources\\Textures\\ball.png";
	DirectX::CreateWICTextureFromFile(pDR->GetD3DDevice(), name2, nullptr, mTexture2.GetAddressOf());
	// �v���~�e�B�u�o�b�`�̍쐬
	mBatch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	mStates = std::make_unique<CommonStates>(device);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	device->CreateBuffer(&bd, nullptr, &mCBuffer);

	mWorld = Matrix::Identity;
	//�G�t�F�N�g�����̉����A���X�g�ɓo�^����B
	int count = 1;
	for (int i = 0; i < count; i++) {
		std::unique_ptr<ShaderSample> effect = std::make_unique<ShaderSample>();
		effect->Create();
		mEffectList.push_back(std::move(effect));
	}
	mFadeStart = false;
	//mDarkScreen = false;
}
//-----------------------------------------------------------------------------------
//	Initialize()
//	�O���ďo���p�̃��b�p�[
//	IN	:	��������		float life
//			�����̒��S		Vector3 pos
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::Initialize(float life, Vector3 pos)
{
	//InitializeAverage(life, pos);
	if (mFadeStart)
	{
		mAlpaNum = 0.0f;
		mDarkScreen = true;
	}
	else
	{
		mAlpaNum = 3.0f;
		mDarkScreen = false;
	}
}
//-----------------------------------------------------------------------------------
//	InitializeRandom()
//	�����_���ȕ����ɔ�ԃG�t�F�N�g���g���Ƃ��̏�����
//	IN	:	��������		float life
//			�����̒��S		Vector3 pos
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::InitializeRandom(float life, Vector3 pos)
{
	int range = 100;

	//life,pos,vel �̒l��m_effect������������
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		Vector3 vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
		while (vel.Length() < 0.03f)
		{
			vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
		}

		(*ite)->Initialize(life, pos, vel);
	}

}
//-----------------------------------------------------------------------------------
//	InitializeAverage()
//	���Ԋu�ɔz�u���Ĕ�ԃG�t�F�N�g���g���Ƃ��̏�����
//	IN	:	��������		float life
//			�����̒��S		Vector3 pos
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::InitializeAverage(float life, Vector3 pos)
{
	const float RAD = XM_PI * 2.0f;
	int num = 0;
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		float size = (float)mEffectList.size();
		//Vector3 vel = Vector3(cos(RAD * num / size + XM_PI / 2.0f), sin(RAD * num / size + XM_PI / 2.0f), 0);
		
		int range = 100;

		Vector3 vel = Vector3(
			-0.5f,
			((rand() % (range * 2)) - range) / (float)range * 0.1f,
			((rand() % (range * 2)) - range) / (float)range * 0.1f
		);
		vel *= 1.0f;
		(*ite)->Initialize(life, pos, vel);
		num++;
	}
}
//-----------------------------------------------------------------------------------
//	InitializeCone()
//	�~����ɔ�ԃG�t�F�N�g���g���Ƃ��̏�����
//	IN	:	��������		float life
//			�����̒��S		Vector3 pos
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::InitializeCone(float life, Vector3 pos)
{
	//�G�t�F�N�g�̒��S��
	Vector3 dir = Vector3(-3, 1, -5);

	//���̒l�𐳋K������
	dir.Normalize();

	//�O�ςɎg�����߂̃J�����ʒu�i�{���͊O����󂯓n����Ɨǂ��j
	Vector3 camera = mCamera;


	//life,pos,vel �̒l��m_effect������������
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		//���ƃJ��������O�ς����߂�
		Vector3 d = dir.Cross(camera);
		//�O�ς𐳋K������
		d.Normalize();
		//�O�ς̒l�ƒ��S�������Z����
		d = dir + d * (float)sin(rand()) * 0.3f;
		//�l���傫���̂�0.1�{
		d *= 0.1f;
		(*ite)->Initialize(life, pos, d);
	}
}
//-----------------------------------------------------------------------------------
//	Update()
//	�X�V����
//	IN	:	�^�C�}�[		DX::StepTimer timer
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::Update(DX::StepTimer timer)
{
	float time = float(timer.GetTotalSeconds());
	mVertex.clear();
	//timer��n����m_effect�̍X�V�������s��
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		(*ite)->Update(timer);

		//mWorld = Matrix::CreateScale(sin(time));
		
		VertexPositionColorTexture vertex;
		Vector3 pos = (*ite)->GetPosition();
		vertex = VertexPositionColorTexture(pos, Vector4::One, Vector2(0.0f, 0.0f));
		mVertex.push_back(vertex);
	}


	// �L�[���͏����擾����
	auto keyState = DirectX::Keyboard::Get().GetState();
	
	if (mAlpaNum < 0.0f)
	{
		// ���炢
		mAlpaNum = 0.0f;
		mDarkScreen = true;
	}
	else if (mAlpaNum > 3.0f)
	{
		// ���邢
		mAlpaNum = 3.0f;
		mDarkScreen = false;
	}

	if (mFadeStart == false)
	{
		mAlpaNum += 0.05f;
	}
	else
	{
		mAlpaNum -= 0.05f;
	}
}
//-----------------------------------------------------------------------------------
//	Render()
//	�`�揈��
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::Render()
{
	int i = 0;
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		this->ShaderDraw(i, ite);
	
		i++;
	}
}

// �`��̏ڍאݒ�
void ShaderManager::ShaderDraw(int num, std::list<std::unique_ptr<ShaderSample>>::iterator ite)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	auto context = pDR->GetD3DDeviceContext();

	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.matView = mView.Transpose();
	cbuff.matProj = mProj.Transpose();
	cbuff.matWorld = mWorld.Transpose();
	//cbuff.Diffuse = Vector4(1, 1, 1, 1);

	Vector3 pos = mVertex[num].position;

	// ����������}��--------------------------------------------------
	cbuff.Diffuse = Vector4(pos.x, pos.y, pos.z, mAlpaNum);
	//-----------------------------------------------------------------


	//�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(mCBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { mCBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);

	//�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { mStates->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//�������`��w��
	ID3D11BlendState* blendstate = mStates->NonPremultiplied();


	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(mStates->DepthRead(), 0);
	// �J�����O�͍�����
	context->RSSetState(mStates->CullNone());

	//�V�F�[�_���Z�b�g����
	context->VSSetShader(mVertexShader.Get(), nullptr, 0);
	context->PSSetShader(mPixelShader.Get(), nullptr, 0);
	context->GSSetShader(mGeometryShader.Get(), nullptr, 0);
	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	context->PSSetShaderResources(0, 1, mTexture.GetAddressOf());
	context->PSSetShaderResources(1, 1, mTexture2.GetAddressOf());

	//�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(mInputLayout.Get());

	// �_�|���S����`��
	mBatch->Begin();
	mBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &mVertex[num], 1);
	mBatch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
}



//-----------------------------------------------------------------------------------
//	SetRenderState()
//	�J�����A�r���[�A�ˉe�̔��f
//	IN	:	�J����		DirectX::SimpleMath::Vector3 camera
//			�r���[�s��	DirectX::SimpleMath::Matrix view
//			�ˉe�s��	DirectX::SimpleMath::Matrix proj
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::SetRenderState(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	mTarget = target;
	mCamera = camera;
	mView = view;
	mProj = proj;
	mWorld = Matrix::CreateBillboard(mTarget, mCamera, Vector3::UnitY);
}
//-----------------------------------------------------------------------------------
//	SetGravity()
//	�d�̓t���O
//	IN	:	�d�̓t���O		bool gravity
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::SetGravity(bool gravity)
{
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = mEffectList.begin(); ite != mEffectList.end(); ite++)
	{
		(*ite)->SetGravity(gravity);
	}
}

//-----------------------------------------------------------------------------------
//	Lost()
//	�j��
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderManager::Lost()
{
	mEffectList.clear();
	mInputLayout.Reset();
	mTexture.Reset();
}
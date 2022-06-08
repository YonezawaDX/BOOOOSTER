//-----------------------------------
//	�G�t�F�N�g�Ǘ��N���X	
//	ShaderManager.h
//-----------------------------------
#pragma once

#include "StepTimer.h"

#include <SimpleMath.h>
#include <list>
#include <WICTextureLoader.h>
#include "ShaderSample.h"

class ShaderManager
{
private:

	Microsoft::WRL::ComPtr<ID3D11Buffer>	mCBuffer;
	DirectX::SimpleMath::Matrix			mWorld;
	DirectX::SimpleMath::Matrix			mView;
	DirectX::SimpleMath::Matrix			mProj;
	DirectX::SimpleMath::Vector3		mCamera;
	DirectX::SimpleMath::Vector3		mTarget;

	//�o�b�`�G�t�F�N�g
	std::unique_ptr<DirectX::AlphaTestEffect> mBatchEffect;
	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> mBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> mStates;
	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture2;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> mGeometryShader;

	//�G�t�F�N�g�̃|�C���^
	std::list<std::unique_ptr<ShaderSample>>	mEffectList;
	std::vector<DirectX::VertexPositionColorTexture>  mVertex;

	// �b���i�[
	float mSecond;

	float mAlpaNum;

	//Fade�̓C���������͂����Ƃ�
	bool mFadeStart;

	// ��ʂ��Â��Ȃ�����
	bool mDarkScreen;
public:

	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

	void Create();
	//Create()�őS�������͒����̂ŕ���
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos);
	void InitializeAverage(float life, DirectX::SimpleMath::Vector3 pos);
	void InitializeRandom(float life, DirectX::SimpleMath::Vector3 pos);
	void InitializeCone(float life, DirectX::SimpleMath::Vector3 pos);

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	void Update(DX::StepTimer timer);
	void Render();
	void ShaderDraw(int num, std::list<std::unique_ptr<ShaderSample>>::iterator ite);

	void SetRenderState(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	void Lost();

	void SetGravity(bool gravity);

	// Fade���X�^�[�g�����
	void SetFade(bool fade) { mFadeStart = fade; }

	bool GetDarkScreen() { return mDarkScreen; }
};
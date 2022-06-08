/*
�v���C�V�[��
*/
#pragma once

#include <CommonStates.h>
#include <Model.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <VertexTypes.h>

#include "../IScene.h"
#include "Game/GameMain.h"

#include "../ShaderManager.h"

// �O���錾
class GridFloor;
class Camera;
class Stage;
class Player;
class FollowCamera;
class Coin;
class EffectManager;
class PlayScene : public IScene
{
private:

	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> mCommonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

	// �f�o�b�O�J����
	std::unique_ptr<Camera> mCamera;
	std::unique_ptr<FollowCamera> mFollowCamera;
	
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> mPrimitiveBatch;

	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> mBasicEffect;

	// �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
	
	
	
	// ���f��
	std::unique_ptr<DirectX::Model> mModel;
	// ���[���h���W
	DirectX::SimpleMath::Matrix mModelWorld;
	unsigned int mFps;

	// �X�e�[�W
	std::unique_ptr<Stage> mStage;

	// �v���C���[
	std::unique_ptr<Player> mPlayer;

	// �R�C��
	std::vector<std::unique_ptr<Coin>> mCoin;
	// �R�C���̊l���p
	std::vector<bool> mCoinFlag;
	// �l�������R�C���̗�
	float mCoinCount;

	// Fade�̏����p
	std::unique_ptr<ShaderManager> mEffect;

	bool mFadeEnd;
	
public:
	// �R���X�g���N�^
	PlayScene();

	// �f�X�g���N�^
	~PlayScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;
};
/*
���U���g�V�[��
*/
#pragma once

#include <CommonStates.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "../IScene.h"
#include "Game/GameMain.h"
#include "Game/ShaderManager.h"

class ResultScene : public IScene
{
private:

	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> mCommonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;

	// Fade�̏����p
	std::unique_ptr<ShaderManager> mEffect;

public:

	// �R���X�g���N�^
	ResultScene();

	// �f�X�g���N�^
	~ResultScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;
};
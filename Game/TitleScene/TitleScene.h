/*
�^�C�g���V�[��
*/
#pragma once

#include <CommonStates.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "../IScene.h"
#include "Game/GameMain.h"

#include "Game/ShaderManager.h"

class TitleScene : public IScene
{
private:
	// �萔--------------------------------------------------
	static const int PUTH_Z_X;		// ������X���W
	static const int PUTH_Z_Y;		// ������Y���W

	static const float TITLE_BACK_X;		// �w�i��X���W
	static const float TITLE_BACK_Y;		// �w�i��Y���W

	static const float TITLE_X;		// title������X���W
	static const float TITLE_Y;		// title������Y���W
	//-------------------------------------------------------
private:

	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> mCommonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture2;

	// Fade�̏����p
	std::unique_ptr<ShaderManager> mEffect;

public:

	// �R���X�g���N�^
	TitleScene();

	// �f�X�g���N�^
	~TitleScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;
};
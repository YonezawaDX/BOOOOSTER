/*
�v���C���[�N���X
*/
#pragma once
#include <CommonStates.h>
#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <StepTimer.h>

// �O���錾
class Model3D;
class Stage;
class MapCollision;
class EffectManager;
class FollowCamera;
class Player
{
public:
	// �萔--------------------------------
	static const float GRAVITY;			// �d��
	static const float PLAYER_LIFE;		// �v���C���[�̗̑�
	
	static const float PLAYER_START_POS_X; // �v���C���[�̏������WX
	static const float PLAYER_START_POS_Y; // �v���C���[�̏������WY

	static const float PLAYER_JUMP;			// �W�����v���̓���

	static const float BOOSTER_AMOUNT_MAX; // �u�[�X�^�[�̎c��
	static const float BOOSTER_SPEED;		// �u�[�X�^�[�̃X�s�[�h
	static const float BOOSTER_SIDE_SPEED;// �u�[�X�^�[�̉��X�s�[�h
	static const float BOOSTER_RECOVERY;	// �u�[�X�^�[�̉񕜃X�s�[�h
	
	static const float GRAVITY_LIMIT; // �d�͂̌��E�l

	static const float PLAYER_SIZE;		// �v���C���[�̃T�C�Y
	static const float LEG_ROTATE_SPEED; // ���̉�]���x

	static const float INVIENCIBLE_TIME; // ���G����
	static const int DRAW_BLINKING_TIME; // ���G����(�_��)

	static const float PLAYER_DAMAGE; // �v���C���[���󂯂�_���[�W
	static const float PLAYER_DEATH; // �v���C���[�̑����_���[�W

	static const float PLAYER_SPEED; //	�v���C���[�̑����_���[�W
	//-------------------------------------

	// �v���C���[�̌����Ə��
	enum class ePlayerDirection :int
	{
		NONE,
		LEFT,
		RIGHT,
		BOOST_LEFT,
		BOOST_RIGHT,
		BOOST_TOP,
		BOOST_DOWN,
	};
public:
	// �֐�-----------------------------------
	// �R���X�g���N�^
	Player(Stage* pStage);
	// �f�X�g���N�^
	~Player();

	// ������
	void Initialize();
	// �X�V
	void Update(float time, const DX::StepTimer& timer);
	// �`��
	void Draw(DirectX::CommonStates* commonStates, FollowCamera* camera);
	// �I������
	void Finalize();

	// �W�����v�̏���
	void Jump(DirectX::Keyboard::State key ,float& velX);

	// �n�ɑ���ڂ��Ă���Ƃ��̈ړ�����
	void GrandMove(DirectX::Keyboard::State key, float& velX);

	// �L�����N�^���}�b�v�Ƃ̓����蔻����l�����Ȃ���ړ�
	int CharMove(float* posX, float* posY, float* gravity,
		float velX, float velY, float size, bool* jumpFlag);

	// �Q�b�^�[---------------------------------------------------------------
	// Player���W
	DirectX::SimpleMath::Vector3 GetPlayerPos() { return mPlayerPos; }
	// �d��
	float GetGravity() { return mGravity;}
	// �u�[�X�^�[�̎c��
	float GetBoostTime() { return mBoostTime; }
	// �v���C���[�̌����Ə��
	int GetPlayerDirection(){ return static_cast<int>(mPlayerDirection); }
	// �v���C���[�̗̑�
	float GetPlayerLife() { return mPlayerLife; }
private:
	// �ϐ�------------------------------------------------------
	// �L�[�{�[�h�X�e�[�g�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker mKeyTracker;
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> mCommonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;

	// Player�̍��W
	DirectX::SimpleMath::Matrix mWorld;
	DirectX::SimpleMath::Vector3 mPlayerPos;
	// Player�̃��f��
	std::unique_ptr<Model3D> mPlayerModel3D;
	std::unique_ptr<Model3D> mPlayerLegModel3D;
	// Stage�̎擾
	Stage* mpStage;

	// �����蔻��
	std::unique_ptr<MapCollision> mMapCollision;

	// �v���C���[�̍��WXY
	float mPosX;
	float mPosY;

	// �|����d��(�W�����v�p)
	float mGravity;
	
	// �d�͂��������Ă��Ȃ��t���O(��Ƀu�[�X�^�[�̉��ړ����Ɏg�p)
	bool mNoGrav;
	// �n�ʂɑ������Ă��邩�̃t���O
	bool mJumpFlag;
	float mJumpTime;

	// �u�[�X�^�[�c��
	float mBoostTime;
	// �u�[�X�^�[���g����t���O
	bool mBoostFlag;

	// �v���C���[�̗̑�
	float mPlayerLife;
	// �v���C���[�̖��G����
	float mPlayerLifeTime;
	// �v���C���[�̖��G���t���O
	bool mPlayerLifeNon;


	float mCount;

	// �v���C���[�̌����Ă������
	ePlayerDirection mPlayerDirection;
	// �G�t�F�N�g
	std::unique_ptr<EffectManager> mEffect;
};
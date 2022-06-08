#include "pch.h"
#include "Player.h"
#include "Model3D.h"
#include "Stage.h"
#include "Libraries/MyLibraries/Collision.h"

#include "Libraries/MyLibraries/FollowCamera.h"
#include "Game/EffectManager.h"

// �萔-----------------------------------------------------------------------
const float Player::GRAVITY = -0.01f;            // �d��
const float Player::PLAYER_LIFE = 100.0f;        // �v���C���[�̗̑�

const float Player::PLAYER_START_POS_X = 5.0f;   // �v���C���[�̏������WX
const float Player::PLAYER_START_POS_Y = -10.0f; // �v���C���[�̏������WY

const float Player::PLAYER_JUMP = 0.25f;			// �W�����v���̓���

const float Player::BOOSTER_AMOUNT_MAX = 100.0f; // �u�[�X�^�[�̎c��
const float Player::BOOSTER_SPEED = 0.20f;       // �u�[�X�^�[�̃X�s�[�h
const float Player::BOOSTER_SIDE_SPEED = 0.18f;  // �u�[�X�^�[�̉��̃X�s�[�h
const float Player::BOOSTER_RECOVERY = 1.0f;	 // �u�[�X�^�[�̉񕜃X�s�[�h

const float Player::GRAVITY_LIMIT = -0.18f;      // �d�͂̌��E�l

const float Player::PLAYER_SIZE = 0.8f;		     // �v���C���[�̃T�C�Y
const float Player::LEG_ROTATE_SPEED = 200.0f;   // ���̉�]���x

const float Player::INVIENCIBLE_TIME = 50.0f;    // ���G����
const int   Player::DRAW_BLINKING_TIME = 5;      // ���G����(�_��)

const float Player::PLAYER_DAMAGE = 10.1f; // �v���C���[���󂯂�_���[�W
const float Player::PLAYER_DEATH = -1.0f; // �v���C���[�̑����_���[�W

const float Player::PLAYER_SPEED = 0.1f; // �v���C���[�̕����X�s�[�h
//---------------------------------------------------------------------------

using namespace DirectX;
using namespace DirectX::SimpleMath;
Player::Player(Stage* pStage)
	: mWorld(DirectX::SimpleMath::Matrix::Identity)
	, mPlayerPos(DirectX::SimpleMath::Vector3::Zero)
	, mPosX(0.0f)
    , mPosY(0.0f)
    , mpStage(pStage)
    , mJumpFlag(false)
    , mGravity(0.0f)
    , mJumpTime(0.0f)
    , mBoostTime(0.0f)
    , mBoostFlag(false)
    , mNoGrav(false)
    , mPlayerDirection(Player::ePlayerDirection::RIGHT)
    , mPlayerLife(0.0f)
    , mCount(0.0f)
    , mPlayerLifeTime(0.0f)
    , mPlayerLifeNon(false)
{
}

Player::~Player()
{
}

// ������
void Player::Initialize()
{
	// Player�̃��f����ݒ肷��
	mPlayerModel3D = std::make_unique<Model3D>();
	mPlayerModel3D->Initialize(L"Resources/Models/RoboBody.cmo");
    mPlayerLegModel3D = std::make_unique<Model3D>();
    mPlayerLegModel3D->Initialize(L"Resources/Models/RoboLeg.cmo");
    // �����蔻��ϐ��̏�����
    mMapCollision = std::make_unique<MapCollision>(mpStage);

    // �v���C���[�̏����ʒu
    mPosX = PLAYER_START_POS_X;
    mPosY = PLAYER_START_POS_Y;

    // �u�[�X�^�[�̎c��MAX
    mBoostTime = BOOSTER_AMOUNT_MAX;

    // �v���C���[�̗̑͂̏�����
    mPlayerLife = PLAYER_LIFE;

    // �G�t�F�N�g�̐���
    mEffect = std::make_unique<EffectManager>();
    mEffect->Create();
    // �G�t�F�N�g�̏�����
    mEffect->Initialize(0.1f, mPlayerPos);

    // �v���C���[�̍��W�Ɉړ��������
    mPlayerPos = Vector3(mPosX, mPosY, 0.0f);

    // �v���C���[�̃��f���̈ʒu��ς���
    mWorld = Matrix::CreateTranslation(mPlayerPos);
}

// �X�V
void Player::Update(float time, const DX::StepTimer& timer)
{
	// �L�[���͏����擾����
	auto keyState = DirectX::Keyboard::Get().GetState();
    mKeyTracker.Update(keyState);

    // �v���C���[�̌��������ȊO�������ꍇ�i�V�ɂ���
    if ((mPlayerDirection != ePlayerDirection::LEFT) && (mPlayerDirection != ePlayerDirection::RIGHT))
    {
        mPlayerDirection = ePlayerDirection::NONE;
    }
    // ���x
    Vector3 vel = Vector3::Zero;

    // �n�ɑ���ڂ��Ă���Ƃ��̈ړ�����
    this->GrandMove(keyState, vel.x);
    // �W�����v�̏���
    this->Jump(keyState, vel.x);

    // ���E�ɔ��ł��Ȃ��Ƃ������d�͂�������
    if (!mNoGrav)
    {
        mGravity += GRAVITY;
    }
    vel.y = mGravity;
    mNoGrav = false;
    if (mGravity < GRAVITY_LIMIT)
    {
        mGravity = GRAVITY_LIMIT;
    }

    // �ړ��ʂɊ�Â��ăL�����N�^�̍��W���ړ�
    CharMove(&mPosX, &mPosY, &mGravity, vel.x, vel.y, PLAYER_SIZE, &mJumpFlag);
    
    // �v���C���[�̍��W�Ɉړ��������
	mPlayerPos = Vector3(mPosX, mPosY, 0.0f);

    // �v���C���[�̃��f���̈ʒu��ς���
	mWorld = Matrix::CreateTranslation(mPlayerPos);


    // �v���C���[�̌������m�F
    bool playerDireLeft = 
        (
            mPlayerDirection == ePlayerDirection::LEFT ||
            mPlayerDirection == ePlayerDirection::BOOST_LEFT
         );

    bool playerDireRight = 
        (
            mPlayerDirection == ePlayerDirection::RIGHT ||
            mPlayerDirection == ePlayerDirection::BOOST_RIGHT
         );
    
    // ���f���̌����ύX
    if (playerDireLeft)
    {
        // �Ђ���Ɍ���
        mPlayerModel3D->SetModelRotationY(180.0f);
    }
    else if (playerDireRight)
    {
        // �E�Ɍ���
        mPlayerModel3D->SetModelRotationY(0.0f);
    }

    // �v���C���̋r�̃��f������]������
    mPlayerLegModel3D->SetModelRotationY(time * LEG_ROTATE_SPEED);

    // �G�t�F�N�g�̕��������߂�
    mEffect->SetEffectDirection(mPlayerDirection);
    // �G�t�F�N�g
    mEffect->Update(timer);

    // ���G����
    if (mPlayerLifeTime > INVIENCIBLE_TIME)
    {
        mPlayerLifeTime = 0.0f;
        mPlayerLifeNon = false;
    }
}

// �`��
void Player::Draw(DirectX::CommonStates* commonStates, FollowCamera* camera)
{
    // �v���C���[�̃u�[�X�g�g�p���̃G�t�F�N�g�`��Z�b�g
     mEffect->SetRenderState(
         camera->GetEyePosition() - mPlayerPos,
         camera->GetViewMatrix() + mWorld,
         camera->GetProjectionMatrix()
     );

    // �G�t�F�N�g�̕`��
    mEffect->Render();

    // �_�Ŏ��̕`��ƒʏ펞�̕`��
    if (static_cast<int>(mPlayerLifeTime) % DRAW_BLINKING_TIME == 0)
    {
        mPlayerModel3D->Draw(commonStates, &mWorld, camera);
        mPlayerLegModel3D->Draw(commonStates, &mWorld, camera);
    }


}

// �I��
void Player::Finalize()
{
}

// �W�����v�̏���
void Player::Jump(DirectX::Keyboard::State key ,float& velX)
{
    // �W�����v�̔���
    if (mJumpFlag == false && mKeyTracker.IsKeyPressed(Keyboard::Z))
    {
        mGravity += PLAYER_JUMP;
        mJumpFlag = true;
        mCount++;
    }

    // �u�[�X�g�̔���
    else if (mJumpFlag == true && mKeyTracker.pressed.Z)
    {
        mBoostFlag = true;
    }

    // �u�[�X�^�[����
    else if (mJumpFlag && mBoostFlag && mBoostTime > 0.0f && key.Z)
    {
        // ��ɔ��
        if (key.Up)
        {
            mGravity = BOOSTER_SPEED;

            mBoostTime--;
            mNoGrav = true;
            mPlayerDirection = ePlayerDirection::BOOST_TOP;
        }
        // ���ɔ��
        else if (key.Down)
        {
            mGravity = -BOOSTER_SPEED;
            mBoostTime--;
            mNoGrav = true;
            mPlayerDirection = ePlayerDirection::BOOST_DOWN;
        }
        // ���ɔ��
        else if (key.Left)
        {
            velX += -BOOSTER_SIDE_SPEED;

            // ���ɔ��ł��邽�ߗ����Ȃ�
            mGravity = 0.0f;

            mBoostTime--;
            mNoGrav = true;
            mPlayerDirection = ePlayerDirection::BOOST_LEFT;
        }
        // �E�ɔ��
        else if (key.Right)
        {
            velX += BOOSTER_SIDE_SPEED;

            // ���ɔ��ł��邽�ߗ����Ȃ�
            mGravity = 0.0f;
            mBoostTime--;
            mNoGrav = true;
            mPlayerDirection = ePlayerDirection::BOOST_RIGHT;
        }
        mCount = 0.0f;
    }
}
// �n�ɑ���ڂ��Ă���Ƃ��̈ړ�����
void Player::GrandMove(DirectX::Keyboard::State key, float& velX)
{
    // ���ɍs��
    if (key.Left)
    {
        velX += -PLAYER_SPEED;
        mPlayerDirection = ePlayerDirection::LEFT;
    }
    // �E�ɍs��
    else if (key.Right)
    {
        velX += PLAYER_SPEED;
        mPlayerDirection = ePlayerDirection::RIGHT;
    }
}

// �L�����̓��� 
int Player::CharMove(
    float* posX, float* posY,
    float* gravity,
    float velX, float velY,
    float size,
    bool* jumpFlag
)
{
    float dummy = 0.0F;
    float hsize;

    // �L�����N�^�̍���A�E��A�����A�E�������������蔻��̂���
    // �}�b�v�ɏՓ˂��Ă��邩���ׁA�Փ˂��Ă�����␳����

    // �����̃T�C�Y���Z�o
    hsize = size * 0.5F;

    // �悸�㉺�ړ����������Ń`�F�b�N
    {
        // �����̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
        if (mMapCollision->MapHitCheck(*posX - hsize, *posY - hsize, &dummy, &velY) == 3) *gravity = 0.0f;

        // �E���̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
        if (mMapCollision->MapHitCheck(*posX + hsize, *posY - hsize, &dummy, &velY) == 3) *gravity = 0.0f;

        // ����̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
        if (mMapCollision->MapHitCheck(*posX - hsize, *posY + hsize, &dummy, &velY) == 4) *gravity *= 0.0f;

        // �E��̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
        if (mMapCollision->MapHitCheck(*posX + hsize, *posY + hsize, &dummy, &velY) == 4) *gravity *= 0.0f;

        // �㉺�ړ����������Z
        *posY += velY;
    }

    // ��ɍ��E�ړ����������Ń`�F�b�N
    {
        // �����̃`�F�b�N
        mMapCollision->MapHitCheck(*posX - hsize, *posY + hsize, &velX, &dummy);

        // �E���̃`�F�b�N
        mMapCollision->MapHitCheck(*posX + hsize, *posY + hsize, &velX, &dummy);

        // ����̃`�F�b�N
        mMapCollision->MapHitCheck(*posX - hsize, *posY - hsize, &velX, &dummy);

        // �E��̃`�F�b�N
        mMapCollision->MapHitCheck(*posX + hsize, *posY - hsize, &velX, &dummy);

        // ���E�ړ����������Z
        *posX += velX;
    }


    // �ڒn����
    {
        // �L�����N�^�̍����ƉE���̉��ɒn�ʂ����邩���ׂ�
        if(mpStage->GetChipParam(*posX - size * 0.5F, (*posY + size * 0.5f - 1.0f) * -1.0f) > 0||
            mpStage->GetChipParam(*posX + size * 0.5F, (*posY + size * 0.5f - 1.0f) * -1.0f) > 0)
        {
            // ���ꂪ�݂�����ڒn���ɂ���
            *jumpFlag = false;
            *gravity = 0.0F;
            mBoostFlag = false;
            if (mBoostTime <= BOOSTER_AMOUNT_MAX)
            {
                mBoostTime += BOOSTER_RECOVERY;
            }

        }
        else
        {
           *jumpFlag = true;
        }

        // �v���C���[�̖��G���Ԃ̓_��
        if (mPlayerLifeNon)
        {
            mPlayerLifeTime++;
            return 0;
        }

        // ���̞��̔���
        bool leftBottom =  mpStage->GetChipParam(*posX - size * 0.5f, (*posY + size * 0.5f - 1.0f) * -1.0f) == static_cast<int>(Stage::eMapType::Spike);
        bool rightBottom = mpStage->GetChipParam(*posX + size * 0.5f, (*posY + size * 0.5f - 1.0f) * -1.0f) == static_cast<int>(Stage::eMapType::Spike);
        
        // �_���[�W����
        // ��
        if (leftBottom||rightBottom)
        {
            mPlayerLife-= PLAYER_DAMAGE;
            mPlayerLifeNon = true;
        }

        // �E�̞��̔���
        bool right = mpStage->GetChipParam((*posX - size * 0.5f) + 0.1f, -(*posY + size * 0.5f)) == static_cast<int>(Stage::eMapType::Spike_Right);
        bool right2 = mpStage->GetChipParam((*posX + size * 0.5f) + 0.1f, -(*posY + size * 0.5f)) == static_cast<int>(Stage::eMapType::Spike_Right);
    
        if (right|| right2)
        {
            mPlayerLife -= PLAYER_DAMAGE;
            mPlayerLifeNon = true;
        }

        // ���̞��̔���
        bool left = mpStage->GetChipParam((*posX - size * 0.5f) - 0.1f, -(*posY + size * 0.5f)) == static_cast<int>(Stage::eMapType::Spike_Left);
        bool left2 = mpStage->GetChipParam((*posX + size * 0.5f) - 0.1f, -(*posY + size * 0.5f)) == static_cast<int>(Stage::eMapType::Spike_Left);

        if (left || left2)
        {
            mPlayerLife -= PLAYER_DAMAGE;
            mPlayerLifeNon = true;
        }
        
        // ��̞��̔���
        bool leftUp = mpStage->GetChipParam(*posX - size * 0.5f,  (*posY + size * 0.5f + 0.1f) * -1.0f) == static_cast<int>(Stage::eMapType::Spike_Top);
        bool rightUp = mpStage->GetChipParam(*posX + size * 0.5f, (*posY + size * 0.5f + 0.1f) * -1.0f) == static_cast<int>(Stage::eMapType::Spike_Top);

        if (leftUp || rightUp)
        {
            mPlayerLife -= PLAYER_DAMAGE;
            mPlayerLifeNon = true;
        }
    
        // ����
        bool rightD = mpStage->GetChipParam((*posX - size * 0.5f) + 0.1f, -(*posY + size * 0.5f)) == static_cast<int>(Stage::eMapType::Spike_Death);
        bool rightD2 = mpStage->GetChipParam((*posX + size * 0.5f) + 0.1f, -(*posY + size * 0.5f)) == static_cast<int>(Stage::eMapType::Spike_Death);

        if (rightD || rightD2)
        {
            mPlayerLife = PLAYER_DEATH;
            mPlayerLifeNon = true;
        }

    }
    // �I��
    return 0;
}
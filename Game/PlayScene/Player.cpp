#include "pch.h"
#include "Player.h"
#include "Model3D.h"
#include "Stage.h"
#include "Libraries/MyLibraries/Collision.h"

#include "Libraries/MyLibraries/FollowCamera.h"
#include "Game/EffectManager.h"

// 定数-----------------------------------------------------------------------
const float Player::GRAVITY = -0.01f;            // 重力
const float Player::PLAYER_LIFE = 100.0f;        // プレイヤーの体力

const float Player::PLAYER_START_POS_X = 5.0f;   // プレイヤーの初期座標X
const float Player::PLAYER_START_POS_Y = -10.0f; // プレイヤーの初期座標Y

const float Player::PLAYER_JUMP = 0.25f;			// ジャンプ時の動き

const float Player::BOOSTER_AMOUNT_MAX = 100.0f; // ブースターの残量
const float Player::BOOSTER_SPEED = 0.20f;       // ブースターのスピード
const float Player::BOOSTER_SIDE_SPEED = 0.18f;  // ブースターの横のスピード
const float Player::BOOSTER_RECOVERY = 1.0f;	 // ブースターの回復スピード

const float Player::GRAVITY_LIMIT = -0.18f;      // 重力の限界値

const float Player::PLAYER_SIZE = 0.8f;		     // プレイヤーのサイズ
const float Player::LEG_ROTATE_SPEED = 200.0f;   // 足の回転速度

const float Player::INVIENCIBLE_TIME = 50.0f;    // 無敵時間
const int   Player::DRAW_BLINKING_TIME = 5;      // 無敵時間(点滅)

const float Player::PLAYER_DAMAGE = 10.1f; // プレイヤーが受けるダメージ
const float Player::PLAYER_DEATH = -1.0f; // プレイヤーの即死ダメージ

const float Player::PLAYER_SPEED = 0.1f; // プレイヤーの歩くスピード
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

// 初期化
void Player::Initialize()
{
	// Playerのモデルを設定する
	mPlayerModel3D = std::make_unique<Model3D>();
	mPlayerModel3D->Initialize(L"Resources/Models/RoboBody.cmo");
    mPlayerLegModel3D = std::make_unique<Model3D>();
    mPlayerLegModel3D->Initialize(L"Resources/Models/RoboLeg.cmo");
    // 当たり判定変数の初期化
    mMapCollision = std::make_unique<MapCollision>(mpStage);

    // プレイヤーの初期位置
    mPosX = PLAYER_START_POS_X;
    mPosY = PLAYER_START_POS_Y;

    // ブースターの残量MAX
    mBoostTime = BOOSTER_AMOUNT_MAX;

    // プレイヤーの体力の初期化
    mPlayerLife = PLAYER_LIFE;

    // エフェクトの生成
    mEffect = std::make_unique<EffectManager>();
    mEffect->Create();
    // エフェクトの初期化
    mEffect->Initialize(0.1f, mPlayerPos);

    // プレイヤーの座標に移動分入れる
    mPlayerPos = Vector3(mPosX, mPosY, 0.0f);

    // プレイヤーのモデルの位置を変える
    mWorld = Matrix::CreateTranslation(mPlayerPos);
}

// 更新
void Player::Update(float time, const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	auto keyState = DirectX::Keyboard::Get().GetState();
    mKeyTracker.Update(keyState);

    // プレイヤーの向きが左以外だった場合ナシにする
    if ((mPlayerDirection != ePlayerDirection::LEFT) && (mPlayerDirection != ePlayerDirection::RIGHT))
    {
        mPlayerDirection = ePlayerDirection::NONE;
    }
    // 速度
    Vector3 vel = Vector3::Zero;

    // 地に足を接いているときの移動処理
    this->GrandMove(keyState, vel.x);
    // ジャンプの処理
    this->Jump(keyState, vel.x);

    // 左右に飛んでいないときだけ重力をかける
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

    // 移動量に基づいてキャラクタの座標を移動
    CharMove(&mPosX, &mPosY, &mGravity, vel.x, vel.y, PLAYER_SIZE, &mJumpFlag);
    
    // プレイヤーの座標に移動分入れる
	mPlayerPos = Vector3(mPosX, mPosY, 0.0f);

    // プレイヤーのモデルの位置を変える
	mWorld = Matrix::CreateTranslation(mPlayerPos);


    // プレイヤーの向きを確認
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
    
    // モデルの向き変更
    if (playerDireLeft)
    {
        // ひだりに向く
        mPlayerModel3D->SetModelRotationY(180.0f);
    }
    else if (playerDireRight)
    {
        // 右に向く
        mPlayerModel3D->SetModelRotationY(0.0f);
    }

    // プレイヤの脚のモデルを回転させる
    mPlayerLegModel3D->SetModelRotationY(time * LEG_ROTATE_SPEED);

    // エフェクトの方向を決める
    mEffect->SetEffectDirection(mPlayerDirection);
    // エフェクト
    mEffect->Update(timer);

    // 無敵時間
    if (mPlayerLifeTime > INVIENCIBLE_TIME)
    {
        mPlayerLifeTime = 0.0f;
        mPlayerLifeNon = false;
    }
}

// 描画
void Player::Draw(DirectX::CommonStates* commonStates, FollowCamera* camera)
{
    // プレイヤーのブースト使用時のエフェクト描画セット
     mEffect->SetRenderState(
         camera->GetEyePosition() - mPlayerPos,
         camera->GetViewMatrix() + mWorld,
         camera->GetProjectionMatrix()
     );

    // エフェクトの描画
    mEffect->Render();

    // 点滅時の描画と通常時の描画
    if (static_cast<int>(mPlayerLifeTime) % DRAW_BLINKING_TIME == 0)
    {
        mPlayerModel3D->Draw(commonStates, &mWorld, camera);
        mPlayerLegModel3D->Draw(commonStates, &mWorld, camera);
    }


}

// 終了
void Player::Finalize()
{
}

// ジャンプの処理
void Player::Jump(DirectX::Keyboard::State key ,float& velX)
{
    // ジャンプの判定
    if (mJumpFlag == false && mKeyTracker.IsKeyPressed(Keyboard::Z))
    {
        mGravity += PLAYER_JUMP;
        mJumpFlag = true;
        mCount++;
    }

    // ブーストの判定
    else if (mJumpFlag == true && mKeyTracker.pressed.Z)
    {
        mBoostFlag = true;
    }

    // ブースター処理
    else if (mJumpFlag && mBoostFlag && mBoostTime > 0.0f && key.Z)
    {
        // 上に飛ぶ
        if (key.Up)
        {
            mGravity = BOOSTER_SPEED;

            mBoostTime--;
            mNoGrav = true;
            mPlayerDirection = ePlayerDirection::BOOST_TOP;
        }
        // 下に飛ぶ
        else if (key.Down)
        {
            mGravity = -BOOSTER_SPEED;
            mBoostTime--;
            mNoGrav = true;
            mPlayerDirection = ePlayerDirection::BOOST_DOWN;
        }
        // 左に飛ぶ
        else if (key.Left)
        {
            velX += -BOOSTER_SIDE_SPEED;

            // 横に飛んでいるため落ちない
            mGravity = 0.0f;

            mBoostTime--;
            mNoGrav = true;
            mPlayerDirection = ePlayerDirection::BOOST_LEFT;
        }
        // 右に飛ぶ
        else if (key.Right)
        {
            velX += BOOSTER_SIDE_SPEED;

            // 横に飛んでいるため落ちない
            mGravity = 0.0f;
            mBoostTime--;
            mNoGrav = true;
            mPlayerDirection = ePlayerDirection::BOOST_RIGHT;
        }
        mCount = 0.0f;
    }
}
// 地に足を接いているときの移動処理
void Player::GrandMove(DirectX::Keyboard::State key, float& velX)
{
    // 左に行く
    if (key.Left)
    {
        velX += -PLAYER_SPEED;
        mPlayerDirection = ePlayerDirection::LEFT;
    }
    // 右に行く
    else if (key.Right)
    {
        velX += PLAYER_SPEED;
        mPlayerDirection = ePlayerDirection::RIGHT;
    }
}

// キャラの動き 
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

    // キャラクタの左上、右上、左下、右下部分が当たり判定のある
    // マップに衝突しているか調べ、衝突していたら補正する

    // 半分のサイズを算出
    hsize = size * 0.5F;

    // 先ず上下移動成分だけでチェック
    {
        // 左下のチェック、もしブロックの上辺に着いていたら落下を止める
        if (mMapCollision->MapHitCheck(*posX - hsize, *posY - hsize, &dummy, &velY) == 3) *gravity = 0.0f;

        // 右下のチェック、もしブロックの上辺に着いていたら落下を止める
        if (mMapCollision->MapHitCheck(*posX + hsize, *posY - hsize, &dummy, &velY) == 3) *gravity = 0.0f;

        // 左上のチェック、もしブロックの下辺に当たっていたら落下させる
        if (mMapCollision->MapHitCheck(*posX - hsize, *posY + hsize, &dummy, &velY) == 4) *gravity *= 0.0f;

        // 右上のチェック、もしブロックの下辺に当たっていたら落下させる
        if (mMapCollision->MapHitCheck(*posX + hsize, *posY + hsize, &dummy, &velY) == 4) *gravity *= 0.0f;

        // 上下移動成分を加算
        *posY += velY;
    }

    // 後に左右移動成分だけでチェック
    {
        // 左下のチェック
        mMapCollision->MapHitCheck(*posX - hsize, *posY + hsize, &velX, &dummy);

        // 右下のチェック
        mMapCollision->MapHitCheck(*posX + hsize, *posY + hsize, &velX, &dummy);

        // 左上のチェック
        mMapCollision->MapHitCheck(*posX - hsize, *posY - hsize, &velX, &dummy);

        // 右上のチェック
        mMapCollision->MapHitCheck(*posX + hsize, *posY - hsize, &velX, &dummy);

        // 左右移動成分を加算
        *posX += velX;
    }


    // 接地判定
    {
        // キャラクタの左下と右下の下に地面があるか調べる
        if(mpStage->GetChipParam(*posX - size * 0.5F, (*posY + size * 0.5f - 1.0f) * -1.0f) > 0||
            mpStage->GetChipParam(*posX + size * 0.5F, (*posY + size * 0.5f - 1.0f) * -1.0f) > 0)
        {
            // 足場が在ったら接地中にする
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

        // プレイヤーの無敵時間の点滅
        if (mPlayerLifeNon)
        {
            mPlayerLifeTime++;
            return 0;
        }

        // 下の棘の判定
        bool leftBottom =  mpStage->GetChipParam(*posX - size * 0.5f, (*posY + size * 0.5f - 1.0f) * -1.0f) == static_cast<int>(Stage::eMapType::Spike);
        bool rightBottom = mpStage->GetChipParam(*posX + size * 0.5f, (*posY + size * 0.5f - 1.0f) * -1.0f) == static_cast<int>(Stage::eMapType::Spike);
        
        // ダメージ判定
        // 床
        if (leftBottom||rightBottom)
        {
            mPlayerLife-= PLAYER_DAMAGE;
            mPlayerLifeNon = true;
        }

        // 右の棘の判定
        bool right = mpStage->GetChipParam((*posX - size * 0.5f) + 0.1f, -(*posY + size * 0.5f)) == static_cast<int>(Stage::eMapType::Spike_Right);
        bool right2 = mpStage->GetChipParam((*posX + size * 0.5f) + 0.1f, -(*posY + size * 0.5f)) == static_cast<int>(Stage::eMapType::Spike_Right);
    
        if (right|| right2)
        {
            mPlayerLife -= PLAYER_DAMAGE;
            mPlayerLifeNon = true;
        }

        // 左の棘の判定
        bool left = mpStage->GetChipParam((*posX - size * 0.5f) - 0.1f, -(*posY + size * 0.5f)) == static_cast<int>(Stage::eMapType::Spike_Left);
        bool left2 = mpStage->GetChipParam((*posX + size * 0.5f) - 0.1f, -(*posY + size * 0.5f)) == static_cast<int>(Stage::eMapType::Spike_Left);

        if (left || left2)
        {
            mPlayerLife -= PLAYER_DAMAGE;
            mPlayerLifeNon = true;
        }
        
        // 上の棘の判定
        bool leftUp = mpStage->GetChipParam(*posX - size * 0.5f,  (*posY + size * 0.5f + 0.1f) * -1.0f) == static_cast<int>(Stage::eMapType::Spike_Top);
        bool rightUp = mpStage->GetChipParam(*posX + size * 0.5f, (*posY + size * 0.5f + 0.1f) * -1.0f) == static_cast<int>(Stage::eMapType::Spike_Top);

        if (leftUp || rightUp)
        {
            mPlayerLife -= PLAYER_DAMAGE;
            mPlayerLifeNon = true;
        }
    
        // 即死
        bool rightD = mpStage->GetChipParam((*posX - size * 0.5f) + 0.1f, -(*posY + size * 0.5f)) == static_cast<int>(Stage::eMapType::Spike_Death);
        bool rightD2 = mpStage->GetChipParam((*posX + size * 0.5f) + 0.1f, -(*posY + size * 0.5f)) == static_cast<int>(Stage::eMapType::Spike_Death);

        if (rightD || rightD2)
        {
            mPlayerLife = PLAYER_DEATH;
            mPlayerLifeNon = true;
        }

    }
    // 終了
    return 0;
}
/*
プレイヤークラス
*/
#pragma once
#include <CommonStates.h>
#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <StepTimer.h>

// 前方宣言
class Model3D;
class Stage;
class MapCollision;
class EffectManager;
class FollowCamera;
class Player
{
public:
	// 定数--------------------------------
	static const float GRAVITY;			// 重力
	static const float PLAYER_LIFE;		// プレイヤーの体力
	
	static const float PLAYER_START_POS_X; // プレイヤーの初期座標X
	static const float PLAYER_START_POS_Y; // プレイヤーの初期座標Y

	static const float PLAYER_JUMP;			// ジャンプ時の動き

	static const float BOOSTER_AMOUNT_MAX; // ブースターの残量
	static const float BOOSTER_SPEED;		// ブースターのスピード
	static const float BOOSTER_SIDE_SPEED;// ブースターの横スピード
	static const float BOOSTER_RECOVERY;	// ブースターの回復スピード
	
	static const float GRAVITY_LIMIT; // 重力の限界値

	static const float PLAYER_SIZE;		// プレイヤーのサイズ
	static const float LEG_ROTATE_SPEED; // 足の回転速度

	static const float INVIENCIBLE_TIME; // 無敵時間
	static const int DRAW_BLINKING_TIME; // 無敵時間(点滅)

	static const float PLAYER_DAMAGE; // プレイヤーが受けるダメージ
	static const float PLAYER_DEATH; // プレイヤーの即死ダメージ

	static const float PLAYER_SPEED; //	プレイヤーの即死ダメージ
	//-------------------------------------

	// プレイヤーの向きと状態
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
	// 関数-----------------------------------
	// コンストラクタ
	Player(Stage* pStage);
	// デストラクタ
	~Player();

	// 初期化
	void Initialize();
	// 更新
	void Update(float time, const DX::StepTimer& timer);
	// 描画
	void Draw(DirectX::CommonStates* commonStates, FollowCamera* camera);
	// 終了処理
	void Finalize();

	// ジャンプの処理
	void Jump(DirectX::Keyboard::State key ,float& velX);

	// 地に足を接いているときの移動処理
	void GrandMove(DirectX::Keyboard::State key, float& velX);

	// キャラクタをマップとの当たり判定を考慮しながら移動
	int CharMove(float* posX, float* posY, float* gravity,
		float velX, float velY, float size, bool* jumpFlag);

	// ゲッター---------------------------------------------------------------
	// Player座標
	DirectX::SimpleMath::Vector3 GetPlayerPos() { return mPlayerPos; }
	// 重力
	float GetGravity() { return mGravity;}
	// ブースターの残量
	float GetBoostTime() { return mBoostTime; }
	// プレイヤーの向きと状態
	int GetPlayerDirection(){ return static_cast<int>(mPlayerDirection); }
	// プレイヤーの体力
	float GetPlayerLife() { return mPlayerLife; }
private:
	// 変数------------------------------------------------------
	// キーボードステートトラッカー
	DirectX::Keyboard::KeyboardStateTracker mKeyTracker;
	// コモンステート
	std::unique_ptr<DirectX::CommonStates> mCommonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;

	// Playerの座標
	DirectX::SimpleMath::Matrix mWorld;
	DirectX::SimpleMath::Vector3 mPlayerPos;
	// Playerのモデル
	std::unique_ptr<Model3D> mPlayerModel3D;
	std::unique_ptr<Model3D> mPlayerLegModel3D;
	// Stageの取得
	Stage* mpStage;

	// 当たり判定
	std::unique_ptr<MapCollision> mMapCollision;

	// プレイヤーの座標XY
	float mPosX;
	float mPosY;

	// 掛かる重力(ジャンプ用)
	float mGravity;
	
	// 重力がかかっていないフラグ(主にブースターの横移動時に使用)
	bool mNoGrav;
	// 地面に足がついているかのフラグ
	bool mJumpFlag;
	float mJumpTime;

	// ブースター残量
	float mBoostTime;
	// ブースターが使えるフラグ
	bool mBoostFlag;

	// プレイヤーの体力
	float mPlayerLife;
	// プレイヤーの無敵時間
	float mPlayerLifeTime;
	// プレイヤーの無敵中フラグ
	bool mPlayerLifeNon;


	float mCount;

	// プレイヤーの向いている方向
	ePlayerDirection mPlayerDirection;
	// エフェクト
	std::unique_ptr<EffectManager> mEffect;
};
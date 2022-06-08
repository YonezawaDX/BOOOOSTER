/*
コインクラス
*/
#pragma once
#include <SimpleMath.h>
#include <CommonStates.h>

class FollowCamera;
class Stage;
class Player;
class Model3D;
class Coin
{
public:
	// 定数用-------------------------------------
	static const float MODEL_ROTATE_SPEED;
public:
	// 関数----------------------------------------
	// コンストラクタ
	Coin();
	//デストラクタ
	~Coin();

	// 初期化
	void Initialize(DirectX::SimpleMath::Vector3 pos);
	// 更新
	void Update(float time);
	// 描画
	void Draw(DirectX::CommonStates* commonStates, FollowCamera* camera);
	// 終了処理
	void Finalize();

	// コインの座標を渡す
	DirectX::SimpleMath::Vector3 GetCoinPos()const { return mPos; }
private:
	// 変数用---------------------------------------------
	// コイン自体の処理用
	// コインの座標
	DirectX::SimpleMath::Vector3 mPos;
	// コインのモデル
	std::unique_ptr<Model3D> mCoinModel3D;
	// コインのワールド座標
	DirectX::SimpleMath::Matrix mCoinWorld;

};

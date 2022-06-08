/*
Stageクラス
*/
#pragma once
#include <vector>
#include <CommonStates.h>
#include <SimpleMath.h>

class FollowCamera;
class Model3D;

class Stage
{
public:
	// 定数-----------------------------------------------------------------
	static const float CHIP_SIZE;			// ブロックのサイズ

	static const int SPIKE_INTERVAL_TIME;	// 即死棘の迫ってくる速さ
	
	static const float SPIKE_INVERSION;		// 棘を反転させる
	static const float SPIKE_NINTY;			// 棘を90度曲げる
	//--------------------------------------------------------------------------
	// マップチップの種類
	enum class eMapType : int
	{
		Coin = -2,
		None = -1,
		Floor,		// 0
		Wall,		// 1
		Spike,		// 2
		Spike_Top,	// 3
		Spike_Left,	// 4
		Spike_Right,// 5
		Gool,		// 6
		Spike_Death,// 7
		MapNum		// 8	
	};

private:


	// データメンバ
	int mRowNum;								// 行数
	int mColumnNum;								// 列数
	std::vector<std::vector<eMapType>> mMap;	// vectorの2次元配列
	std::unique_ptr<Model3D> mModel3D[static_cast<int>(eMapType::MapNum)];	// 3Dモデルの配列

	// コインの座標を渡す
	std::vector<DirectX::SimpleMath::Matrix> mCoinWorid;

	// マップ変更用タイマー
	int mTimer;

	// マップを変換する
	int mChangeColumn;

	// ゴールの座標
	DirectX::SimpleMath::Vector3 mGoolPos;
public:
	// コンストラクタ、デストラクタ
	Stage();
	~Stage();

	// 行数を取得する
	int GetMapRow() const { return mRowNum; }

	// 列数を取得する
	int GetMapColumn() const { return mColumnNum; }

	// マップ配列の参照を取得する
	std::vector<std::vector<eMapType>>& GetMap() { return mMap; }

	// マップを変更するための変換後進
	void Update(float time);

	// 描画
	void Draw(DirectX::CommonStates* commonStates, FollowCamera* camera);

	// マップチップの値を取得する関数
	int GetChipParam(float posX, float posY);

	// 新しいマップをセットする
	void SetMapDestruction(std::vector<std::vector<eMapType>> map) { mMap = map; }

	// ゴールの座標をゲットする
	DirectX::SimpleMath::Vector3 GetGoolPos() const { return mGoolPos; }

private:
	// マップデータを読み込む
	bool LoadCSV(const wchar_t* mapFile);

	// マップモデルのLoad
	void LoadModel3D();
};
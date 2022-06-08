/*
Stageクラス
*/
#include "pch.h"
#include "Stage.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <Effects.h>
#include <SimpleMath.h>
#include "DeviceResources.h"
#include "Libraries/MyLibraries/FollowCamera.h"
#include "Model3D.h"
#include <Keyboard.h>

// 定数---------------------------------------------------------------
const int Stage::SPIKE_INTERVAL_TIME = 50;	// 棘の迫ってくる時間

const float Stage::SPIKE_INVERSION = 180.0f;		// 棘を反転させる
const float Stage::SPIKE_NINTY = 90.0f;			// 棘を90度曲げる
//--------------------------------------------------------------------

// コンストラクタ
Stage::Stage()
	:mColumnNum(0)
	,mRowNum(0)
	,mTimer(0)
	,mChangeColumn(0)
	,mGoolPos(DirectX::SimpleMath::Vector3::Zero)
{
	this->LoadCSV(L"Resources/CSV/Map.csv");
	this->LoadModel3D();
}

// デストラクタ
Stage::~Stage()
{
	// マップを消す
	mMap.clear();
}

// 更新
void Stage::Update(float time)
{
	// 警告よけ
	UNREFERENCED_PARAMETER(time);

	// 迫ってくる棘
	if (mTimer > SPIKE_INTERVAL_TIME)
		mTimer = 0;
	mTimer++;
}

// マップの描画
void Stage::Draw(DirectX::CommonStates* commonStates, FollowCamera* camera)
{

	// 赤い棘を迫らせる
	if (mTimer == SPIKE_INTERVAL_TIME)
	{
		for (int i = 0; i < mRowNum; i++)
		{
			// 即死棘をマップの端に生成
			mMap[i][mChangeColumn] = eMapType::Spike_Death;
			if (mChangeColumn > 0)
			{
				// 一つ前のブロックを消す
				mMap[i][mChangeColumn - 1] = eMapType::Floor;
			}
		
		}
		// ひとつ下にずらす
		mChangeColumn++;
	}

	for (int i = 0; i < mRowNum; i++)
	{
		for (int j = 0; j < mColumnNum; j++)
		{	
			// ブロックの描画位置をずらす
			DirectX::SimpleMath::Vector3 mapPos = DirectX::SimpleMath::Vector3(j + 0.5f, -i - 0.5f, 0.0f);
			DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(mapPos);
			
			// 表示する3Dオブジェクトの種類
			switch (mMap[i][j])
			{
				// なにもない
				case eMapType::Floor:
					break;
				// 壁
				case eMapType::Wall:
					mModel3D[static_cast<int>(eMapType::Wall)]->Draw(commonStates, &world, camera);
					break;
				// 棘
				case eMapType::Spike:
					mModel3D[static_cast<int>(eMapType::Spike)]->Draw(commonStates, &world, camera);
					break;				 
				// コイン
				case eMapType::Coin:
					break;
				
				// 上の棘
				case eMapType::Spike_Top:
					mModel3D[static_cast<int>(eMapType::Spike_Top)]->SetModelRotationZ(SPIKE_INVERSION);
					mModel3D[static_cast<int>(eMapType::Spike_Top)]->Draw(commonStates, &world, camera);
					break;
				// 左の棘
				case eMapType::Spike_Left:
					mModel3D[static_cast<int>(eMapType::Spike_Left)]->SetModelRotationZ(-SPIKE_NINTY);
					mModel3D[static_cast<int>(eMapType::Spike_Left)]->Draw(commonStates, &world, camera);
					break;
				// 右の棘
				case eMapType::Spike_Right:
					mModel3D[static_cast<int>(eMapType::Spike_Right)]->SetModelRotationZ(SPIKE_NINTY);
					mModel3D[static_cast<int>(eMapType::Spike_Right)]->Draw(commonStates, &world, camera);
					break;
				
				// ゴール
				case eMapType::Gool:
					mModel3D[static_cast<int>(eMapType::Gool)]->Draw(commonStates, &world, camera);
					mGoolPos = mapPos;
					break;
				
				// 即死棘 
				case eMapType::Spike_Death:
					mModel3D[static_cast<int>(eMapType::Spike_Death)]->SetModelRotationZ(-SPIKE_NINTY);
					mModel3D[static_cast<int>(eMapType::Spike_Death)]->Draw(commonStates, &world, camera);
					break;
				default:
					assert(!"マップチップの方が正しくありません");
					break;
			}
		}
	}
}


// マップチップの値を取得する関数
int Stage::GetChipParam(float posX, float posY)
{
	// 整数変換用変数
	int x, y;

	// 整数値へ変換
	x = static_cast<int>(posX);
	y = static_cast<int>(posY);

	// マップからはみ出ていたら 0 を返す
	//To do 直したほうが良さそう（カメラの範囲ならある。マップの大きさとかない）
	if (x >= mColumnNum || y >= mRowNum || x < 0 || y < 0) return static_cast<int>(Stage::eMapType::None);

	// 指定の座標に該当するマップの情報を返す
	return static_cast<int>(mMap[y][x]);
}

// CSVファイルからのマップデータの読み込み
bool Stage::LoadCSV(const wchar_t* mapFile)
{
	// ローカル変数
	std::ifstream ifs;		// 入力ファイルストリーム
	std::istringstream iss;	// 入力文字列ストリーム
	std::string line;		// 1行分の文字列
	std::string data;		// (カンマで区切られた)1データ分の文字列
	
	// ファイルを開く
	ifs.open(mapFile);

	// ファイルを開かなければ、失敗
	if (ifs.fail()) { return false; }

	// 行数を読み込む
	std::getline(ifs, line);
	mRowNum = std::stoi(line);

	// 列数を読み込む
	std::getline(ifs, line);
	mColumnNum = std::stoi(line);

	// 2次元配列のサイズをデータの大きさに合わせて調整する
	mMap.resize(mRowNum, std::vector<eMapType>(mColumnNum));

	// CSVからマップデータを読み込む
	for (int i = 0; i < mRowNum; i++)
	{
		// 一行分の文字列を読み込む
		std::getline(ifs, line);
		// issの状態をクリア -> しないとハマる
		iss.clear(std::istringstream::goodbit);
		// 文字列から文字列ストリームに変換
		iss.str(line);

		// カンマ区切りで文字列をint型に変換し、配列に格納する
		for (int j = 0; j < mColumnNum; j++)
		{
			std::getline(iss, data, ',');
			mMap[i][j] = static_cast<eMapType>(std::stoi(data));
		}
	}

	// ファイルを閉じる
	ifs.close();

	// ファイルの読み込み成功
	return true;
}

// マップのモデルを読み込む
void Stage::LoadModel3D()
{
	// デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();

	// モデルデータの入っているフォルダの指定
	std::unique_ptr<DirectX::EffectFactory> factory = std::make_unique<DirectX::EffectFactory>(device);
	factory->SetDirectory(L"Resources/Models");

	//3Dモデル生成
	for (unsigned int i = 0; i < static_cast<int>(eMapType::MapNum); i++)
	{

		mModel3D[i] = std::make_unique<Model3D>();
	}
	mModel3D[0]->Initialize(L"Resources/Models/Floor.cmo");
	mModel3D[1]->Initialize(L"Resources/Models/Wall.cmo");
	mModel3D[2]->Initialize(L"Resources/Models/spike.cmo");
	mModel3D[3]->Initialize(L"Resources/Models/spike.cmo");
	mModel3D[4]->Initialize(L"Resources/Models/spike.cmo");
	mModel3D[5]->Initialize(L"Resources/Models/spike.cmo");
	mModel3D[6]->Initialize(L"Resources/Models/Gool3.cmo");
	mModel3D[7]->Initialize(L"Resources/Models/spikeDeath.cmo");
}

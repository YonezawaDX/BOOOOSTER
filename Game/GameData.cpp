#include "pch.h"
#include "GameData.h"

GameData g_gameData;


/*----------------------------------------------
初期化
----------------------------------------------*/
void GameData::Initialize(void)
{
	// スコアの値を0にする
	gScore = 0;
}
/*
ゲームデータ
*/
#pragma once

struct GameData
{
	// 変数 ===================================
	// プレイモード
	int gScore;

	// 関数 ===================================

	// 初期化
	void Initialize(void);
};

// どこでも使えるようにする
extern GameData g_gameData;

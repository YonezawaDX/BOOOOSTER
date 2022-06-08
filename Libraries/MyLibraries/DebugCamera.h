//
// DebugCamera.h
//
#pragma once

#include <Mouse.h>

#include "Camera.h"

/// <summary>
/// デバッグ用カメラクラス
/// </summary>
class DebugCamera : public Camera
{
public:
	static const float DEFAULT_CAMERA_DISTANCE;	// カメラの距離

private:
	float mAngleX;								// 縦回転角
	float mAngleY;								// 横回転角
	int mPrevX, mPrevY;							// 前回のマウス座標(X,Y)
	int mScrollWheelValue;						// マウスホイールのスクロール値

public:
	// コンストラクタ
	DebugCamera();
	// デストラクタ
	~DebugCamera();

	// 更新処理
	void Update();

private:
	// マウスポインタのドラッグ開始位置からの変位(相対値)
	void DraggedDistance(int x, int y);

	// ビュー行列の算出
	void CalculateViewMatrix() override;
};

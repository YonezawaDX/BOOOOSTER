/*
Modelクラス
*/
#pragma once
#include <SimpleMath.h>
#include "StepTimer.h"
#include <CommonStates.h>
#include <Model.h>

// 前方宣言
class FollowCamera;

class Model3D
{
public:
	// 関数------------------------------------
	// コンストラクタ
	Model3D();
	// デストラクタ
	~Model3D() = default;

	// 初期化
	void Initialize(const wchar_t* fileName);

	// 描画
	void Draw(
		DirectX::CommonStates* commonStates,
		DirectX::SimpleMath::Matrix* world,
		FollowCamera* camera
	);

	// 終了処理
	void Finalize();

	// モデルを回転させる
	void SetModelRotationY(float rotate) { mRotateY = rotate; }
	void SetModelRotationZ(float rotate) { mRotateZ = rotate; }

private:
	// モデル
	std::unique_ptr<DirectX::Model> mModel;

	// 回転軸の変数YZ
	float mRotateY;
	float mRotateZ;

};
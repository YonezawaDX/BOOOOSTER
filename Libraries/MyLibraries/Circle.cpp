//
// Circle.cpp
//
#include "pch.h"
#include "Circle.h"

// 円を描画
void Circle::Draw(
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
	const DirectX::SimpleMath::Vector3& center,	// 円の中心座標
	const float radius,							// 半径
	const float rate,							// 円を何％描画するか？(0.0f--1.0f)
	const DirectX::FXMVECTOR& color,			// 色
	const float alpha							// 半透明度(0.0f--1.0f)
)
{
	// アルファ値の反映
	DirectX::SimpleMath::Vector4 col = color;
	col.w = alpha;

	// 表示角(ラジアン)
	// 時計回りに頂点を指定するので、(× -1.0f)
	float radian = DirectX::XM_2PI * rate * (-1.0f);

	// 頂点座標の作成
	for (int i = 0; i < NUM_VERTICES; i++)
	{
		// Y軸基準
		// 外周の頂点の設定(Y軸プラスの垂直方向から、時計回りで頂点を作成)
		mVertices[i].x = 0.0f + radius * -sinf((radian / DIV) * i);	
		mVertices[i].y = 0.0f + radius * cosf((radian / DIV) * i);
		mVertices[i].z = 0.0f;
	}

	// Y軸基準
	// 指定座標に平行移動③
	DirectX::SimpleMath::Matrix mat = DirectX::SimpleMath::Matrix::CreateTranslation(center);

	// ①② or ③で求めた行列で頂点配列を変換Remaining amount
	for (int i = 0; i < NUM_VERTICES; i++)
	{
		mVertices[i] = DirectX::SimpleMath::Vector3::Transform(mVertices[i], mat);
	}

	// ポリゴンを描画
	for (int i = 0; i < DIV; i++)
	{
		// 頂点を時計回りにポリゴンを描画 -> ポリゴンの表面がカメラから見えている
		batch->DrawTriangle(
			{ center, col },
			{ mVertices[i + 0], color },
			{ mVertices[i + 1], color }
		);
	}
}

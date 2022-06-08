#include "pch.h"
#include "Collision.h"
#include <algorithm>
#include <array>

#include "Game/PlayScene/Stage.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// コンストラクタ
AABB::AABB(const Vector3& min, const Vector3& max)
	:mMin(min)
	,mMax(max)
{
}

// デストラクタ
void AABB::UpdateMinMax(const Vector3& point)
{
	// 各成分を更新する
	mMin = Vector3::Min(mMin, point);
	mMax = Vector3::Max(mMax, point);
}

// 回転後の成分を更新する
void AABB::Rotate(const DirectX::SimpleMath::Quaternion& q)
{
	// ボックスの角の８つの点
	std::array<Vector3, 8> points;

	// 最小の点、常に角と当たる
	points[0] = mMin;

	// 2個のminと1個のmaxによる順列組み合わせ
	points[1] = Vector3(mMax.x, mMin.y, mMin.z);
	points[2] = Vector3(mMin.x, mMax.y, mMin.z);
	points[3] = Vector3(mMin.x, mMin.y, mMax.z);
	
	// 2個のmaxと1個のminによる順列組み合わせ
	points[4] = Vector3(mMin.x, mMax.y, mMax.z);
	points[5] = Vector3(mMax.x, mMin.y, mMax.z);
	points[6] = Vector3(mMax.x, mMax.y, mMin.z);

	// 最大の点は角にある
	points[7] = Vector3(mMax);

	//最初の点を回転する
	Vector3 p = Vector3::Transform(points[0], q);

	// 回転した点でmin,maxをリセットする
	mMin = p;
	mMax = p;

	// 残りの点によってmin,maxを更新
	for (size_t i = 0; i < points.size(); i++)
	{
		p = Vector3::Transform(points[i], q);
		UpdateMinMax(p);
	}

}

bool AABB::Contains(const DirectX::SimpleMath::Vector3& point) const
{
	bool outside = point.x < mMin.x ||
		point.y < mMin.y ||
		point.z < mMin.z ||
		point.x > mMax.x ||
		point.y > mMax.y ||
		point.z > mMax.z;
	// 上記の条件ではない
	return !outside;
}

// 衝突判定
bool AABB::Collision(const AABB& a, const AABB& b)
{
	bool no = a.mMax.x < b.mMin.x ||
		a.mMax.y < b.mMin.y ||
		a.mMax.z < b.mMin.z ||
		b.mMax.x < a.mMin.x ||
		b.mMax.y < a.mMin.y ||
		b.mMax.z < a.mMin.z;
	// 上記の条件が真でなければ、あたっている	
	return !no;
}


// マップとの当たり判定========================================================
MapCollision::MapCollision(Stage* stage)
	:mpStage(stage)
{
}

// 当たったかチェック
int MapCollision::MapHitCheck(float posX, float posY, float* velX, float* velY)
{
    float afX, afY;

    // 移動量を足す
    afX = posX + *velX;
    afY = posY + *velY;

    // 当たり判定のあるブロックに当たっているかチェック
    // TODO 今後当たり判定のあるブロックは出てくる
    if (mpStage->GetChipParam(afX, -afY) > 0&& mpStage->GetChipParam(afX, -afY) != 6)
    {
        float blx, bty, brx, bby;

        // 当たっていたら壁から離す処理を行う

        // ブロックの上下左右の座標を算出
        blx = (float)((int)afX);    // 左辺の X 座標
        brx = (float)((int)afX);    // 右辺の X 座標

        bty = (float)((int)afY);     // 上辺の Z 座標
        bby = (float)((int)afY);    // 下辺の Z 座標

        // 上辺に当たっていた場合
        if (*velY > 0.0F)
        {
            // 移動量を補正する
            *velY = 0.0f;

            // 上辺に当たったと返す
            return 3;
        }

        // 下辺に当たっていた場合
        if (*velY < 0.0F)
        {
            // 移動量を補正する
            *velY = 0.0f;

            // 下辺に当たったと返す
            return 4;
        }

        // 左辺に当たっていた場合
        if (*velX > 0.0F)
        {
            // 移動量を補正する
            *velX = 0.0f;

            // 左辺に当たったと返す
            return 1;
        }

        // 右辺に当たっていた場合
        if (*velX < 0.0F)
        {
            // 移動量を補正する
            *velX = 0.0f;

            // 右辺に当たったと返す
            return 2;
        }

        // ここに来たら適当な値を返す
        return 4;
    }

    // どこにも当たらなかったと返す
    return 0;
}

/*
当たり判定
*/
#pragma once
#include <SimpleMath.h>
#include <vector>

class AABB
{
private:
	DirectX::SimpleMath::Vector3 mMin;
	DirectX::SimpleMath::Vector3 mMax;
public:
	AABB(const DirectX::SimpleMath::Vector3& min,
		const DirectX::SimpleMath::Vector3& max);

	~AABB() = default;

	// ポイントの最小、最大合計を更新
	// モデルをロードしたとき用
	void UpdateMinMax(const DirectX::SimpleMath::Vector3 &point);
	
	// 回転後の当たり判定用
	void Rotate(const DirectX::SimpleMath::Quaternion& q);
	
	// 点との当たり判定用
	bool Contains(const DirectX::SimpleMath::Vector3& point) const;

	// 判定をとる
	bool Collision(const AABB& a, const AABB& b);
};

// ステージとの当たり判定
class Stage;
class MapCollision
{
private:
	Stage* mpStage;
public:
	MapCollision(Stage* stage);
	~MapCollision() = default;

	// マップとの当たり判定( 戻り値 0:当たらなかった  1:左辺に当たった  2:右辺に当たった
	//                                                3:上辺に当たった  4:下辺に当たった
	int MapHitCheck(float posX, float posY, float* velX, float* velY);

};
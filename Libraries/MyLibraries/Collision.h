/*
�����蔻��
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

	// �|�C���g�̍ŏ��A�ő升�v���X�V
	// ���f�������[�h�����Ƃ��p
	void UpdateMinMax(const DirectX::SimpleMath::Vector3 &point);
	
	// ��]��̓����蔻��p
	void Rotate(const DirectX::SimpleMath::Quaternion& q);
	
	// �_�Ƃ̓����蔻��p
	bool Contains(const DirectX::SimpleMath::Vector3& point) const;

	// ������Ƃ�
	bool Collision(const AABB& a, const AABB& b);
};

// �X�e�[�W�Ƃ̓����蔻��
class Stage;
class MapCollision
{
private:
	Stage* mpStage;
public:
	MapCollision(Stage* stage);
	~MapCollision() = default;

	// �}�b�v�Ƃ̓����蔻��( �߂�l 0:������Ȃ�����  1:���ӂɓ�������  2:�E�ӂɓ�������
	//                                                3:��ӂɓ�������  4:���ӂɓ�������
	int MapHitCheck(float posX, float posY, float* velX, float* velY);

};
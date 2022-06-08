#include "pch.h"
#include "Collision.h"
#include <algorithm>
#include <array>

#include "Game/PlayScene/Stage.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �R���X�g���N�^
AABB::AABB(const Vector3& min, const Vector3& max)
	:mMin(min)
	,mMax(max)
{
}

// �f�X�g���N�^
void AABB::UpdateMinMax(const Vector3& point)
{
	// �e�������X�V����
	mMin = Vector3::Min(mMin, point);
	mMax = Vector3::Max(mMax, point);
}

// ��]��̐������X�V����
void AABB::Rotate(const DirectX::SimpleMath::Quaternion& q)
{
	// �{�b�N�X�̊p�̂W�̓_
	std::array<Vector3, 8> points;

	// �ŏ��̓_�A��Ɋp�Ɠ�����
	points[0] = mMin;

	// 2��min��1��max�ɂ�鏇��g�ݍ��킹
	points[1] = Vector3(mMax.x, mMin.y, mMin.z);
	points[2] = Vector3(mMin.x, mMax.y, mMin.z);
	points[3] = Vector3(mMin.x, mMin.y, mMax.z);
	
	// 2��max��1��min�ɂ�鏇��g�ݍ��킹
	points[4] = Vector3(mMin.x, mMax.y, mMax.z);
	points[5] = Vector3(mMax.x, mMin.y, mMax.z);
	points[6] = Vector3(mMax.x, mMax.y, mMin.z);

	// �ő�̓_�͊p�ɂ���
	points[7] = Vector3(mMax);

	//�ŏ��̓_����]����
	Vector3 p = Vector3::Transform(points[0], q);

	// ��]�����_��min,max�����Z�b�g����
	mMin = p;
	mMax = p;

	// �c��̓_�ɂ����min,max���X�V
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
	// ��L�̏����ł͂Ȃ�
	return !outside;
}

// �Փ˔���
bool AABB::Collision(const AABB& a, const AABB& b)
{
	bool no = a.mMax.x < b.mMin.x ||
		a.mMax.y < b.mMin.y ||
		a.mMax.z < b.mMin.z ||
		b.mMax.x < a.mMin.x ||
		b.mMax.y < a.mMin.y ||
		b.mMax.z < a.mMin.z;
	// ��L�̏������^�łȂ���΁A�������Ă���	
	return !no;
}


// �}�b�v�Ƃ̓����蔻��========================================================
MapCollision::MapCollision(Stage* stage)
	:mpStage(stage)
{
}

// �����������`�F�b�N
int MapCollision::MapHitCheck(float posX, float posY, float* velX, float* velY)
{
    float afX, afY;

    // �ړ��ʂ𑫂�
    afX = posX + *velX;
    afY = posY + *velY;

    // �����蔻��̂���u���b�N�ɓ������Ă��邩�`�F�b�N
    // TODO ���㓖���蔻��̂���u���b�N�͏o�Ă���
    if (mpStage->GetChipParam(afX, -afY) > 0&& mpStage->GetChipParam(afX, -afY) != 6)
    {
        float blx, bty, brx, bby;

        // �������Ă�����ǂ��痣���������s��

        // �u���b�N�̏㉺���E�̍��W���Z�o
        blx = (float)((int)afX);    // ���ӂ� X ���W
        brx = (float)((int)afX);    // �E�ӂ� X ���W

        bty = (float)((int)afY);     // ��ӂ� Z ���W
        bby = (float)((int)afY);    // ���ӂ� Z ���W

        // ��ӂɓ������Ă����ꍇ
        if (*velY > 0.0F)
        {
            // �ړ��ʂ�␳����
            *velY = 0.0f;

            // ��ӂɓ��������ƕԂ�
            return 3;
        }

        // ���ӂɓ������Ă����ꍇ
        if (*velY < 0.0F)
        {
            // �ړ��ʂ�␳����
            *velY = 0.0f;

            // ���ӂɓ��������ƕԂ�
            return 4;
        }

        // ���ӂɓ������Ă����ꍇ
        if (*velX > 0.0F)
        {
            // �ړ��ʂ�␳����
            *velX = 0.0f;

            // ���ӂɓ��������ƕԂ�
            return 1;
        }

        // �E�ӂɓ������Ă����ꍇ
        if (*velX < 0.0F)
        {
            // �ړ��ʂ�␳����
            *velX = 0.0f;

            // �E�ӂɓ��������ƕԂ�
            return 2;
        }

        // �����ɗ�����K���Ȓl��Ԃ�
        return 4;
    }

    // �ǂ��ɂ�������Ȃ������ƕԂ�
    return 0;
}

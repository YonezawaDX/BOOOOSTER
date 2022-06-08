//
// Circle.cpp
//
#include "pch.h"
#include "Circle.h"

// �~��`��
void Circle::Draw(
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
	const DirectX::SimpleMath::Vector3& center,	// �~�̒��S���W
	const float radius,							// ���a
	const float rate,							// �~�������`�悷�邩�H(0.0f--1.0f)
	const DirectX::FXMVECTOR& color,			// �F
	const float alpha							// �������x(0.0f--1.0f)
)
{
	// �A���t�@�l�̔��f
	DirectX::SimpleMath::Vector4 col = color;
	col.w = alpha;

	// �\���p(���W�A��)
	// ���v���ɒ��_���w�肷��̂ŁA(�~ -1.0f)
	float radian = DirectX::XM_2PI * rate * (-1.0f);

	// ���_���W�̍쐬
	for (int i = 0; i < NUM_VERTICES; i++)
	{
		// Y���
		// �O���̒��_�̐ݒ�(Y���v���X�̐�����������A���v���Œ��_���쐬)
		mVertices[i].x = 0.0f + radius * -sinf((radian / DIV) * i);	
		mVertices[i].y = 0.0f + radius * cosf((radian / DIV) * i);
		mVertices[i].z = 0.0f;
	}

	// Y���
	// �w����W�ɕ��s�ړ��B
	DirectX::SimpleMath::Matrix mat = DirectX::SimpleMath::Matrix::CreateTranslation(center);

	// �@�A or �B�ŋ��߂��s��Œ��_�z���ϊ�Remaining amount
	for (int i = 0; i < NUM_VERTICES; i++)
	{
		mVertices[i] = DirectX::SimpleMath::Vector3::Transform(mVertices[i], mat);
	}

	// �|���S����`��
	for (int i = 0; i < DIV; i++)
	{
		// ���_�����v���Ƀ|���S����`�� -> �|���S���̕\�ʂ��J�������猩���Ă���
		batch->DrawTriangle(
			{ center, col },
			{ mVertices[i + 0], color },
			{ mVertices[i + 1], color }
		);
	}
}

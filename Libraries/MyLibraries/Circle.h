//
// Circle.h
//
#pragma once
#include <SimpleMath.h>
#include <VertexTypes.h>
#include <PrimitiveBatch.h>

/// <summary>
/// �~���|���S���ŕ`�悷��N���X
/// </summary>
class Circle
{
private:
	static const int DIV = 24;								// �~�ʂ̕�����
	static const int NUM_VERTICES = DIV + 1;				// ���_��
	DirectX::SimpleMath::Vector3 mVertices[NUM_VERTICES];	// ���_���z��

public:
	Circle() = default;
	~Circle() = default;

	// �`�揈��
	void Draw(
		DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		const DirectX::SimpleMath::Vector3& center,
		const float radius,
		const float rate = 1.0f,
		const DirectX::FXMVECTOR& color = DirectX::Colors::White,
		const float alpha = 1.0f
	);
};


//
// Circle.h
//
#pragma once
#include <SimpleMath.h>
#include <VertexTypes.h>
#include <PrimitiveBatch.h>

/// <summary>
/// ‰~‚ğƒ|ƒŠƒSƒ“‚Å•`‰æ‚·‚éƒNƒ‰ƒX
/// </summary>
class Circle
{
private:
	static const int DIV = 24;								// ‰~ŒÊ‚Ì•ªŠ„”
	static const int NUM_VERTICES = DIV + 1;				// ’¸“_”
	DirectX::SimpleMath::Vector3 mVertices[NUM_VERTICES];	// ’¸“_î•ñ”z—ñ

public:
	Circle() = default;
	~Circle() = default;

	// •`‰æˆ—
	void Draw(
		DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		const DirectX::SimpleMath::Vector3& center,
		const float radius,
		const float rate = 1.0f,
		const DirectX::FXMVECTOR& color = DirectX::Colors::White,
		const float alpha = 1.0f
	);
};


#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

class ShaderSample
{
public:


	ShaderSample();
	~ShaderSample();

	void Create();
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 velocity);
	void Update(DX::StepTimer timer);
	void Render();
	void Reset();
	void Lost();

	void SetGravity(bool isGravity) { mIsGravity = isGravity; }
	DirectX::SimpleMath::Vector3 GetPosition() { return mPosition; }

private:

	DX::StepTimer                           mTimer;

	DirectX::SimpleMath::Vector3		mCamera;
	DirectX::SimpleMath::Vector3		mGravity;
	DirectX::SimpleMath::Vector3		mPosition;
	DirectX::SimpleMath::Vector3		mVelocity;
	float								mLife;

	DirectX::SimpleMath::Vector3		mStartPosition;
	DirectX::SimpleMath::Vector3		mStartVelocity;
	float								mStartLife;

	float mWait;

	bool								mIsGravity;


};
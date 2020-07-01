#pragma once

#include "Graphics.h"
#include "Camera.h"

class Player
{
public:
	void ChangePlayerMovementVelocity(DirectX::XMVECTOR velocity);
	void ChangePlayerRotationVelocity(DirectX::XMVECTOR velocity);
	void ChangePlayerLookPitchYaw(DirectX::XMFLOAT2 pitchYaw);

	void Update(float dt);
	DirectX::XMMATRIX GetProjection();

	Camera GetCamera();
private:
	float friction = 0.01f;
	DirectX::XMVECTOR movementVelocity = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMVECTOR position = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMVECTOR rotationVelocity = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMFLOAT2 lookPitchYaw = DirectX::XMFLOAT2(0, 0);

	Camera camera;
};
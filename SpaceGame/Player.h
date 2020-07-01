#pragma once

#include "Graphics.h"
#include "Camera.h"

class Player
{
public:
	void ChangePlayerMovementVelocity(DirectX::XMFLOAT3 velocity);
	void ChangePlayerRotationVelocity(DirectX::XMFLOAT3 velocity);
	void ChangePlayerLookPitchYaw(DirectX::XMFLOAT2 pitchYaw);

	void Update(float dt);
	DirectX::XMMATRIX GetProjection();

	Camera GetCamera();
private:
	float friction = 0.0f;
	DirectX::XMVECTOR movementVelocity = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMVECTOR position = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMFLOAT3 rotationVelocity = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT2 lookPitchYaw = DirectX::XMFLOAT2(0, 0);

	Camera camera;
};
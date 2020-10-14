#pragma once

#include "Graphics.h"
#include "Camera.h"

class Player
{
public:
	void ChangePlayerMovementVelocity(DirectX::XMVECTOR velocity);
	void Rotate(DirectX::XMFLOAT3 rotation);

	void Update(float dt);
	DirectX::XMMATRIX GetProjection();

	Camera* GetCamera();
private:
	float friction = 0.01f;
	DirectX::XMVECTOR velocity = DirectX::XMVectorSet(0, 0, 0, 0);

	Camera camera;
};
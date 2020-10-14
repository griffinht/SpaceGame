#pragma once

#include <DirectXMath.h>
#include "Graphics.h"

class Camera
{
public:
	Camera();

	void SetAspectRatio(float aspectRatio);
	void SetPosition(DirectX::XMVECTOR position);
	void SetFOV(float fov);
	void ChangeFOV(float fov);
	void Translate(DirectX::XMVECTOR translation);
	void TranslateWithRotation(DirectX::XMVECTOR translation);
	void Rotate(DirectX::XMFLOAT3 rotation);

	DirectX::XMMATRIX GetRotation();

	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix();
private:
	DirectX::XMVECTOR position = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0);
	float fov = 1.57f;//90 degrees todo make more accurate?
	float aspectRatio = 16.0f / 9.0f;
};
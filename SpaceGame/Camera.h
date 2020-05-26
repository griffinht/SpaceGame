#pragma once

#include <DirectXMath.h>
#include "Graphics.h"

class Camera
{
public:
	Camera();

	void SetAspectRatio(float aspectRatio);
	void SetPosition(float x, float y, float z);
	void SetRotation(float pitch, float yaw, float roll);
	void SetFOV(float fov);
	void ChangeFOV(float fov);

	void Translate(DirectX::XMFLOAT3 translation);
	void TranslateWithRotation(DirectX::XMFLOAT3 translation);
	void Rotate(DirectX::XMFLOAT3 rot);

	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix();
private:
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(0.0f);
	DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(0.0f);
	DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(0.0f);
	float fov = 1.0f;
	float aspectRatio = 16.0f / 9.0f;
};
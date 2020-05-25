#pragma once

#include <DirectXMath.h>
#include "Graphics.h"

class Camera
{
public:
	Camera();

	void SetPosition(float x, float y, float z);
	void SetRotation(float pitch, float yaw, float roll);
	void SetProjectionRect(DirectX::XMFLOAT4 rect);
	void SetZoom(float zoom);

	void Translate(DirectX::XMFLOAT3 translation);
	void TranslateWithRotation(DirectX::XMFLOAT3 translation);
	void ChangeRotation(float pitch, float yaw, float roll);
	void ChangeZoom(float zoom);

	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix();
private:
	float NormalizeInRange(float norm, float min, float max);
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT4 projectionRect = DirectX::XMFLOAT4(-640.0f, 640.0f, -310.0f, 310.0f);
	float zoom = 1.0f;
};
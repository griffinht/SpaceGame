#pragma once

#include <DirectXMath.h>
#include "Graphics.h"

class Camera
{
public:
	Camera();
	Camera(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation);

	void SetPosition(float x, float y, float z);
	void SetRotation(float pitch, float yaw, float roll);

	void ChangePosition(float x, float y, float z);
	void ChangeRotation(float pitch, float yaw, float roll);

	void Bind(Graphics& graphics);
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
};
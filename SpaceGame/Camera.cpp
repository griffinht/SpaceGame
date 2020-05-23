#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera()
{}

void Camera::SetPosition(float x, float y, float z)
{
	position = DirectX::XMFLOAT3(x, y, z);
}

void Camera::SetRotation(float pitch, float yaw, float roll)
{
	rotation = DirectX::XMFLOAT3(pitch, yaw, roll);
}

void Camera::SetProjectionRect(DirectX::XMFLOAT4 rect)
{
	projectionRect = rect;
}

void Camera::SetZoom(float zoom)
{
	this->zoom = zoom;
}

void Camera::ChangePosition(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
}

void Camera::ChangeRotation(float roll, float pitch, float yaw)
{
	rotation.x = fmod(rotation.x + roll, M_PI);
	rotation.y = fmod(rotation.y + pitch, M_PI);
	rotation.z = fmod(rotation.z + yaw, M_PI);
}

void Camera::ChangeZoom(float zoom)
{
	this->zoom += zoom;
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	using namespace DirectX;

	XMVECTOR Eye = XMVectorSet(position.x, position.y, position.z, 0.0f);

	return XMMatrixLookAtLH(
		Eye,
		Eye + XMVector3Transform(
			XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

DirectX::XMMATRIX Camera::GetProjectionMatrix()
{
	using namespace DirectX;

	return XMMatrixPerspectiveLH(16, 9, 3, 100);
	/*
	return XMMatrixOrthographicOffCenterLH(
		projectionRect.w / zoom,
		projectionRect.x / zoom,
		projectionRect.y / zoom,
		projectionRect.z / zoom,
		0.1f,
		10.0f);
		*/
}

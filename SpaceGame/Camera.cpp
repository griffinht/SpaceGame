#include "Camera.h"

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

void Camera::ChangeRotation(float pitch, float yaw, float roll)
{
	rotation.x += pitch;
	rotation.y += yaw;
	rotation.z += roll;
}

void Camera::ChangeZoom(float zoom)
{
	this->zoom += zoom;
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	using namespace DirectX;

	XMVECTOR Eye = XMVectorSet(position.x, position.y, -1.0f, 0.0f);
	XMVECTOR LookAt = XMVectorSet(position.x, position.y, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	return XMMatrixLookAtLH(Eye, LookAt, Up);
}

DirectX::XMMATRIX Camera::GetProjectionMatrix()
{
	using namespace DirectX;

	return XMMatrixOrthographicOffCenterLH(
		projectionRect.w / zoom,
		projectionRect.x / zoom,
		projectionRect.y / zoom,
		projectionRect.z / zoom,
		0.1f,
		10.0f);
}

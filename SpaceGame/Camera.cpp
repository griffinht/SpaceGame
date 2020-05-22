#include "Camera.h"

Camera::Camera()
	:
	position(DirectX::XMFLOAT3(0, 0, 0)),
	rotation(DirectX::XMFLOAT3(0, 0, 0))
{}

Camera::Camera(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation)
	:
	position(position),
	rotation(rotation)
{}

void Camera::SetPosition(float x, float y, float z)
{
	position = DirectX::XMFLOAT3(x, y, z);
}

void Camera::SetRotation(float pitch, float yaw, float roll)
{
	rotation = DirectX::XMFLOAT3(pitch, yaw, roll);
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

void Camera::Bind(Graphics& graphics)
{
	using namespace DirectX;

	const auto pos = XMLoadFloat3(&position);
	graphics.SetProjection(XMMatrixLookAtLH(
		pos,
		pos + XMVector3Transform(
			XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)), 
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
}

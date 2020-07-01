#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

Camera::Camera()
{}

void Camera::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
}

void Camera::SetPosition(float x, float y, float z)
{
	position = DirectX::XMFLOAT3(x, y, z);
}

void Camera::SetRotation(float pitch, float yaw, float roll)
{
	rotationMatrixX = DirectX::XMMatrixRotationX(pitch);
	rotationMatrixY = DirectX::XMMatrixRotationX(yaw);
	rotationMatrixZ = DirectX::XMMatrixRotationX(roll);
}

void Camera::SetFOV(float fov)
{
	this->fov = fov;
}

void Camera::ChangeFOV(float fov)
{
	this->fov += fov;
	this->fov = std::min(std::max(this->fov, 0.001f), (float) M_PI);
}

void Camera::Translate(DirectX::XMFLOAT3 translation)
{
	position.x += translation.x;
	position.y += translation.y;
	position.z += translation.z;
}

void Camera::TranslateWithRotation(DirectX::XMFLOAT3 translation)
{
	DirectX::XMStoreFloat3(&translation, DirectX::XMVector3Transform(
		DirectX::XMLoadFloat3(&translation),
		rotationMatrixX * rotationMatrixY * rotationMatrixZ
	));

	position.x += translation.x;
	position.y += translation.y;
	position.z += translation.z;
}

void Camera::Rotate(DirectX::XMFLOAT3 rot)//yaw pitch roll, should be between -1 and 1, will be converted to radians
{
	using namespace DirectX;

	XMFLOAT3 floaty;
	XMStoreFloat3(&floaty, XMVector3Transform(
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), 
		rotationMatrixX));
	if (floaty.y < 0)
	{
		rot.y = -rot.y;
	}

	rotationMatrixX *= XMMatrixRotationX(rot.x * M_PI * 2);
	rotationMatrixY *= XMMatrixRotationY(rot.y * M_PI * 2);
	rotationMatrixZ *= XMMatrixRotationZ(rot.z * M_PI * 2);
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	using namespace DirectX;

	XMVECTOR Eye = XMVectorSet(position.x, position.y, position.z, 0.0f);

	return XMMatrixLookAtLH(
		Eye,
		Eye + XMVector3Transform(
			XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			rotationMatrixX * rotationMatrixY * rotationMatrixZ),
		XMVector3Transform(
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
			rotationMatrixX * rotationMatrixY * rotationMatrixZ));
}

DirectX::XMMATRIX Camera::GetProjectionMatrix()
{
	using namespace DirectX;

	return XMMatrixPerspectiveFovLH(fov, aspectRatio, 1, 100);
	//XMMatrixPerspectiveFovLH
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
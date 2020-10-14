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

void Camera::SetPosition(DirectX::XMVECTOR position)
{
	this->position = position;
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

void Camera::Translate(DirectX::XMVECTOR translation)
{
	using namespace DirectX;
	position += translation;
}

void Camera::TranslateWithRotation(DirectX::XMVECTOR translation)
{
	using namespace DirectX;
	position += DirectX::XMVector3Transform(translation, rotation);
}

void Camera::Rotate(DirectX::XMFLOAT3 rotation)
{
	//this finally works! i think this just rotates using local instead of global coords or something, prob not very efficient but whatever it finaly works
	using namespace DirectX;
	this->rotation *= XMMatrixRotationAxis(XMVector3Transform(XMVectorSet(1, 0, 0, 0), this->rotation), rotation.x);
	this->rotation *= XMMatrixRotationAxis(XMVector3Transform(XMVectorSet(0, 1, 0, 0), this->rotation), rotation.y);
	this->rotation *= XMMatrixRotationAxis(XMVector3Transform(XMVectorSet(0, 0, 1, 0), this->rotation), rotation.z);
}
DirectX::XMMATRIX Camera::GetRotation()
{
	return rotation;
}
DirectX::XMMATRIX Camera::GetViewMatrix()
{
	using namespace DirectX;
	return XMMatrixLookAtLH(
		position,
		position + XMVector3Transform(
			XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			rotation),
		XMVector3Transform(
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
			rotation));
}

DirectX::XMMATRIX Camera::GetProjectionMatrix()
{
	using namespace DirectX;
	return XMMatrixPerspectiveFovLH(fov, aspectRatio, 1, 100);
}
#include "Player.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

void Player::ChangePlayerMovementVelocity(DirectX::XMVECTOR velocity)
{
	using namespace DirectX;
	movementVelocity += XMVector3Transform(velocity, XMMatrixRotationRollPitchYawFromVector(rotation));//should player be updated before this???
}

void Player::ChangePlayerRotationVelocity(DirectX::XMVECTOR velocity)
{
	using namespace DirectX;
	rotationVelocity += velocity;
}

void Player::ChangePlayerLookPitchYaw(DirectX::XMFLOAT2 pitchYaw)
{
	lookPitchYaw.x += pitchYaw.x * M_PI * 2;
	lookPitchYaw.y += pitchYaw.y * M_PI * 2;
}

void Player::Update(float dt)
{
	using namespace DirectX;

	rotation += rotationVelocity * dt;

	rotation += rotation * -1 * friction * dt;
	
	position += DirectX::XMVector3Transform(movementVelocity, XMMatrixRotationRollPitchYawFromVector(rotation)) * dt;
}

DirectX::XMMATRIX Player::GetProjection()
{
	using namespace DirectX;

	XMMATRIX view = XMMatrixLookAtLH(
		position,
		position + XMVector3Transform(
			XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			XMMatrixRotationRollPitchYaw(lookPitchYaw.x, lookPitchYaw.y, 0.0f)),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	return view * XMMatrixRotationRollPitchYawFromVector(rotation) * camera.GetProjectionMatrix();
}

Camera Player::GetCamera()
{
	return camera;
}

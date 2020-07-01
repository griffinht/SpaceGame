#include "Player.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

void Player::ChangePlayerMovementVelocity(DirectX::XMFLOAT3 velocity)
{
	using namespace DirectX;
	movementVelocity += XMVector3Transform(XMLoadFloat3(&velocity), XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z));//should player be updated before this???
}

void Player::ChangePlayerRotationVelocity(DirectX::XMFLOAT3 velocity)
{
	using namespace DirectX;
	rotationVelocity.x += velocity.x;
	rotationVelocity.y += velocity.y;
	rotationVelocity.z += velocity.z;
}

void Player::ChangePlayerLookPitchYaw(DirectX::XMFLOAT2 pitchYaw)
{
	lookPitchYaw.x += pitchYaw.x * M_PI * 2;
	lookPitchYaw.y += pitchYaw.y * M_PI * 2;
}

void Player::Update(float dt)
{
	using namespace DirectX;

	rotation.x += rotationVelocity.x * dt;
	rotation.y += rotationVelocity.y * dt;
	rotation.z += rotationVelocity.z * dt;

	rotation.x += rotation.x * -1 * friction * dt;
	rotation.y += rotation.y * -1 * friction * dt;
	rotation.z += rotation.z * -1 * friction * dt;
	
	position += DirectX::XMVector3Transform(movementVelocity, XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)) * dt;
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
	return view * XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) * camera.GetProjectionMatrix();
}

Camera Player::GetCamera()
{
	return camera;
}

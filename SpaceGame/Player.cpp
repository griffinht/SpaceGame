#include "Player.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

void Player::ChangePlayerMovementVelocity(DirectX::XMVECTOR velocity)
{
	using namespace DirectX;
	velocity += XMVector3Transform(velocity, camera.GetRotation());//should player be updated before this???
}

void Player::Rotate(DirectX::XMFLOAT3 rotation)
{
	using namespace DirectX;
	camera.Rotate(XMFLOAT3(rotation.x * M_PI * 2, rotation.y * M_PI * 2, rotation.z * M_PI * 2));
}

void Player::Update(float dt)
{
	using namespace DirectX;

	camera.TranslateWithRotation(velocity * dt);

	velocity += velocity * -1 * friction * dt;
}

DirectX::XMMATRIX Player::GetProjection()
{
	using namespace DirectX;
	return camera.GetViewMatrix() * camera.GetProjectionMatrix();
}

Camera* Player::GetCamera()
{
	return &camera;
}

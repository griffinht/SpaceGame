#pragma once

#include <random>
#include "DrawableBase.h"

class Box : public DrawableBase<Box>
{
public:
	Box(Graphics& graphics, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
	void Update(float dt) override;
	DirectX::XMMATRIX GetTransformXM() const override;
private:
	// position
	float r = 1;
	float roll = 1;
	float pitch = 1;
	float yaw = 1;
	float theta = 1;
	float phi = 1;
	float chi = 1;
	// speed
	float droll = 1;
	float dpitch = 1;
	float dyaw = 1;
	float dtheta = 1;
	float dphi = 1;
	float dchi = 1;
};
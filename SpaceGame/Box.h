#pragma once

#include "Drawable.h"

class Box : public Drawable
{
public:
	Box(Graphics& graphics);
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
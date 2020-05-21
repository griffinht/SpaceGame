#pragma once

#include "Graphics.h"
#include "GraphicsThrowMacros.h"

class Bindable
{
public:
	virtual void Bind(Graphics& gfx) = 0;
	virtual ~Bindable() = default;
protected:
	static ID3D11DeviceContext* GetContext(Graphics& graphics);
	static ID3D11Device* GetDevice(Graphics& graphics);
	static DxgiInfoManager& GetInfoManager(Graphics& graphics);
};
#pragma once

#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformConstantBuffer : public Bindable
{
public:
	TransformConstantBuffer(Graphics& graphics, const Drawable& parent);
	void Bind(Graphics& graphics) override;
private:
	VertexConstantBuffer<DirectX::XMMATRIX> vertexConstantBuffer;
	const Drawable& parent;
};
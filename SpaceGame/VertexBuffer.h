#pragma once

#include "Bindable.h"

class VertexBuffer : public Bindable
{
public:
	VertexBuffer(Graphics& graphics, const Vertex* vBuffer[]);
	void Bind(Graphics& graphics) override;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};
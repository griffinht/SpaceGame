#pragma once

#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(Graphics& graphics, const C* cb) 
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(cb);
		cbd.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &cb;

		HRESULT hr;
		GRAPHICS_THROW_INFO(graphics.pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	}
	void Bind(Graphics& graphics) override
	{
		graphics.pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
private:

	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};
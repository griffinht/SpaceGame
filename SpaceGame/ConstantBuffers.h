#pragma once

#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	void Update(Graphics& graphics, const C& consts)
	{
		D3D11_MAPPED_SUBRESOURCE msr;

		INFO_MANAGER(graphics);
		GRAPHICS_THROW_INFO(GetContext(graphics)->Map(
			pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr
		));
		memcpy(msr.pData, &consts, sizeof(consts));
		GetContext(graphics)->Unmap(pConstantBuffer.Get(), 0u);
	}
	ConstantBuffer(Graphics& graphics, const C& consts)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		
		INFO_MANAGER(graphics);
		GRAPHICS_THROW_INFO(GetDevice(graphics)->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	}
	ConstantBuffer(Graphics& graphics)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;
		
		INFO_MANAGER(graphics);
		GRAPHICS_THROW_INFO(GetDevice(graphics)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& graphics) override
	{
		Bindable::GetContext(graphics)->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& graphics) override
	{
		Bindable::GetContext(graphics)->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};
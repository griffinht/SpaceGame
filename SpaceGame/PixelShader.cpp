#include "PixelShader.h"

PixelShader::PixelShader(Graphics& graphics)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	HRESULT hr;
	GRAPHICS_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	GRAPHICS_THROW_INFO(graphics.pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
}

void PixelShader::Bind(Graphics& graphics)
{
	graphics.pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}

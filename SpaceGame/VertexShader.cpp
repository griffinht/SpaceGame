#include "VertexShader.h"

VertexShader::VertexShader(Graphics& graphics)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	HRESULT hr;
	GRAPHICS_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	GRAPHICS_THROW_INFO(graphics.pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
}

void VertexShader::Bind(Graphics& graphics)
{
	graphics.pContext->VSSetShader(pVertexShader.Get(), 0, 0);
}

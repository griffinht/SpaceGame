#include "PixelShader.h"

PixelShader::PixelShader(Graphics& graphics, const std::wstring& path)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	INFO_MANAGER(graphics);
	GRAPHICS_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
	GRAPHICS_THROW_INFO(GetDevice(graphics)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
}

void PixelShader::Bind(Graphics& graphics)
{
	GetContext(graphics)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}

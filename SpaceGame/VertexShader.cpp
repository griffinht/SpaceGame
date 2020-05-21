#include "VertexShader.h"

VertexShader::VertexShader(Graphics& graphics, const std::wstring& path)
{
	

	INFO_MANAGER(graphics);
	GRAPHICS_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
	GRAPHICS_THROW_INFO(GetDevice(graphics)->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
}

void VertexShader::Bind(Graphics& graphics)
{
	GetContext(graphics)->VSSetShader(pVertexShader.Get(), 0, 0);
}

ID3DBlob* VertexShader::GetBytecode() const
{
	return pBlob.Get();
}

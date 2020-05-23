#include "InputLayout.h"

InputLayout::InputLayout(Graphics& graphics, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
{
	INFO_MANAGER(graphics);
	GRAPHICS_THROW_INFO(GetDevice(graphics)->CreateInputLayout(
		layout.data(), 
		(UINT)layout.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
	));
}

void InputLayout::Bind(Graphics& graphics)
{
	GetContext(graphics)->IASetInputLayout(pInputLayout.Get());
}

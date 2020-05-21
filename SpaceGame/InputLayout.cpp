#include "InputLayout.h"

InputLayout::InputLayout(Graphics& graphics, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	HRESULT hr;
	(graphics.pDevice->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));
}

void InputLayout::Bind(Graphics& graphics)
{
	graphics.pContext->IASetInputLayout(pInputLayout.Get());
}

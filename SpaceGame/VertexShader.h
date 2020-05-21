#pragma once

#include "Bindable.h"
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& graphics, const std::wstring& path);
	void Bind(Graphics& graphics) override;
	ID3DBlob* GetBytecode() const;
protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
};
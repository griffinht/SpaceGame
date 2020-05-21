#pragma once

#include "Bindable.h"
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& graphics, const std::wstring& path);
	void Bind(Graphics& graphics) override;
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};
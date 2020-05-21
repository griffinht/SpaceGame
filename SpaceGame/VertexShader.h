#pragma once

#include "Bindable.h"
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& graphics);
	void Bind(Graphics& graphics) override;
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};
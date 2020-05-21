#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& graphics, const std::vector<unsigned short>& indices);
	void Bind(Graphics& graphics) override;
	UINT GetCount() const;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	UINT count;
};
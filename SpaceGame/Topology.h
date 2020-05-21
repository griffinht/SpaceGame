#pragma once

#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology(Graphics& graphiccs, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(Graphics& graphics) override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};
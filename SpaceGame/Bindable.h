#pragma once

#include "Graphics.h"
#include "EngineException.h"
#include "DXGIInfoManager.h"
#include "GraphicsThrowMacros.h"
#include <DirectXMath.h>

class Bindable
{
public:
	virtual void Bind(Graphics& graphics);
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
	};
protected:
	DxgiInfoManager infoManager;
};
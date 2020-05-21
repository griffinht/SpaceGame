#pragma once

#include "Windows.h"
#include <wrl.h>
#include <vector>
#include <dxgidebug.h>
#include <string>

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager() = default;
	std::vector<std::string> GetMessages() const;
	bool CheckResize();
private:
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
};
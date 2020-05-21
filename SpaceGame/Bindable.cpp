#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(Graphics& graphics)
{
	return graphics.pContext.Get();
}

ID3D11Device* Bindable::GetDevice(Graphics& graphics)
{
	return graphics.pDevice.Get();
}

DxgiInfoManager& Bindable::GetInfoManager(Graphics& graphics)
{
	return graphics.infoManager;
}
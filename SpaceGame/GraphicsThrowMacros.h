#pragma once

//needs HRESULT hr defined somewhere
#define GRAPHICS_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException( __LINE__, __FILE__, hr)
#define GRAPHICS_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)

#if defined _DEBUG
#define GRAPHICS_THROW_INFO(hrcall) if(FAILED(hr=(hrcall))) throw Graphics::HrException( __LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GRAPHICS_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr, infoManager.GetMessages())
#else
#define GRAPHICS_THROW_INFO(hrcall) GRAPHICS_THROW_NOINFO(hrcall)
#endif
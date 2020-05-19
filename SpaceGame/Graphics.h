#pragma once

#include "EngineException.h"
#include <Windows.h>
#include <d3d11_1.h>
#include <dxgi1_2.h>
#include <vector>
#include <wrl.h>
#include "DXGIInfoManager.h"

class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics() = default;
	void Present(UINT syncInterval, UINT flags);
	void Clear(float red, float green, float blue);
	void CreateDevice();
	void CreateResources();
	void SetFullscreenState(bool fullscreen);
	void drawTriangle(float angle, float x, float y);
	void OnDeviceLost();
private:
	HWND hWnd;
	UINT backBufferWidth;
	UINT backBufferHeight;
	D3D_FEATURE_LEVEL featureLevel;
	DxgiInfoManager infoManager;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
public:
	class Exception : public EngineException
	{
		using EngineException::EngineException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMessages = {});
		const char* what() const override;
		const char* GetType() const override;
		HRESULT GetErrorCode() const;
		std::string GetErrorInfo() const;
	private:
		HRESULT hr;
		std::string info;
	};

	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const override;
	};

	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMessages);
		const char* what() const override;
		const char* GetType() const override;
		std::string GetErrorInfo() const;
	private:
		std::string info;
	};
};
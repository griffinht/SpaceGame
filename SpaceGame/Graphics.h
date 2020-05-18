#pragma once

#include "EngineException.h"
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include "DXGIInfoManager.h"

class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics() = default;
	void FlipBuffer();
	void ClearBuffer(float red, float green, float blue);
	void drawTriangle(float angle, float x, float y);
private:
	DxgiInfoManager infoManager;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
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
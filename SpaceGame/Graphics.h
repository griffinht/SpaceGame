#pragma once

#include "EngineException.h"
#include <Windows.h>
#include <d3d11.h>
#include <vector>

class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics();
	void FlipBuffer();
	void ClearBuffer(float red, float green, float blue);
private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;

public:
	class HrException : public EngineException
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

	class InfoException : public EngineException
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
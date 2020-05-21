#include "Graphics.h"
#include <sstream>
#include "Window.h"
#include "GraphicsThrowMacros.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace WRL = Microsoft::WRL;
namespace DX = DirectX;

Graphics::Graphics(HWND hWnd)
	:
	hWnd(hWnd),
	backBufferWidth(1280),
	backBufferHeight(720),
	featureLevel(D3D_FEATURE_LEVEL_11_1)
{
	CreateDevice();
	CreateResources();
	HRESULT hr;
	GRAPHICS_THROW_INFO(pSwap->GetFullscreenState(&wasFullscreen, nullptr));
}

Graphics::~Graphics()
{
	pSwap->SetFullscreenState(false, nullptr);
}

void Graphics::Present(UINT syncInterval, UINT flags)
{
	BOOL fullscreen;
	HRESULT hr;
	GRAPHICS_THROW_INFO(pSwap->GetFullscreenState(&fullscreen, nullptr));
	if (fullscreen != wasFullscreen)
	{
		wasFullscreen = fullscreen;
		ResizeBuffers(0, 0);
	}

	if (FAILED(hr = pSwap->Present(syncInterval, flags)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GRAPHICS_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			GRAPHICS_THROW_INFO(hr);
		}
	}
}

void Graphics::Clear(float red, float green, float blue)
{
	std::lock_guard<std::mutex> lockGuard(mutex);
	const float color[] = { red, green, blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);

	pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), pDSV.Get());
}

void Graphics::CreateDevice()
{
	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static const D3D_FEATURE_LEVEL featureLevels [] =
	{
		// TODO: Modify for supported Direct3D feature levels
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	WRL::ComPtr<ID3D11Device> device;
	WRL::ComPtr<ID3D11DeviceContext> context;
	HRESULT hr;
	GRAPHICS_THROW_INFO(D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		device.ReleaseAndGetAddressOf(),
		&featureLevel,
		context.ReleaseAndGetAddressOf()
	));

	GRAPHICS_THROW_INFO(device.As(&pDevice));
	GRAPHICS_THROW_INFO(context.As(&pContext));

	// TODO: Initialize device dependent objects here (independent of window size).
}

void Graphics::CreateResources()
{
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	pContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	pTarget.Reset();
	pDSV.Reset();
	pContext->Flush();

	HRESULT hr;
	// If the swap chain already exists, resize it, otherwise create one.
	if (pSwap)
	{
		hr = pSwap->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			OnDeviceLost();

			// Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
			// and correctly set up the new device.
			return;
		}
		else
		{
			GRAPHICS_THROW_INFO(hr);
		}
	}
	else
	{
		// First, retrieve the underlying DXGI Device from the D3D Device.
		WRL::ComPtr<IDXGIDevice1> dxgiDevice;
		GRAPHICS_THROW_INFO(pDevice.As(&dxgiDevice));

		// Identify the physical adapter (GPU or card) this device is running on.
		WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
		GRAPHICS_THROW_INFO(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

		// And obtain the factory object that created it.
		WRL::ComPtr<IDXGIFactory2> dxgiFactory;
		GRAPHICS_THROW_INFO(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		// Create a descriptor for the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = backBufferWidth;
		swapChainDesc.Height = backBufferHeight;
		swapChainDesc.Format = backBufferFormat;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = backBufferCount;
		swapChainDesc.SwapEffect = swapEffect;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
		fsSwapChainDesc.Windowed = TRUE;

		// Create a SwapChain from a Win32 window.
		GRAPHICS_THROW_INFO(dxgiFactory->CreateSwapChainForHwnd(
			pDevice.Get(),
			hWnd,
			&swapChainDesc,
			&fsSwapChainDesc,
			nullptr,
			pSwap.ReleaseAndGetAddressOf()
		));

		// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
		//GRAPHICS_THROW_INFO(dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));
	}

	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
	WRL::ComPtr<ID3D11Texture2D> backBuffer;
	GRAPHICS_THROW_INFO(pSwap->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	// Create a view interface on the rendertarget to use on bind.
	GRAPHICS_THROW_INFO(pDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, pTarget.ReleaseAndGetAddressOf()));

	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	WRL::ComPtr<ID3D11Texture2D> depthStencil;
	GRAPHICS_THROW_INFO(pDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	GRAPHICS_THROW_INFO(pDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, pDSV.ReleaseAndGetAddressOf()));

	// TODO: Initialize windows-size dependent objects here.
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, (float)backBufferWidth, (float)backBufferHeight);
	pContext->RSSetViewports(1, &viewport);
}

void Graphics::SetFullscreenState(bool fullscreen)
{
	HRESULT hr;
	GRAPHICS_THROW_INFO(pSwap->SetFullscreenState(fullscreen, nullptr));
}

void Graphics::ResizeBuffers(UINT width, UINT height)
{
	std::lock_guard<std::mutex> lockGuard(mutex);
	if (pSwap)
	{
		if (width > 0)
		{
			backBufferWidth = width;
		}
		if (height > 0)
		{
			backBufferHeight = height;
		}

		pContext->OMSetRenderTargets(0, 0, 0);//idk if this does anything
		pTarget->Release();

		HRESULT hr;
		GRAPHICS_THROW_INFO(pSwap->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));

		WRL::ComPtr<ID3D11Texture2D> backBuffer;
		GRAPHICS_THROW_INFO(pSwap->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));
		GRAPHICS_THROW_INFO(pDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, pTarget.GetAddressOf()));

		CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);
		WRL::ComPtr<ID3D11Texture2D> depthStencil;
		GRAPHICS_THROW_INFO(pDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
		GRAPHICS_THROW_INFO(pDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, pDSV.ReleaseAndGetAddressOf()));

		//this is usually done on Clear()
		pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), NULL);

		CD3D11_VIEWPORT viewport(0.0f, 0.0f, (float)backBufferWidth, (float)backBufferHeight);
		pContext->RSSetViewports(1, &viewport);
	}
}

void Graphics::DrawIndexed(UINT count)
{
	GRAPHICS_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0u, 0u));
}

void Graphics::SetProjection(DirectX::FXMMATRIX projection)
{
	this->projection = projection;
}

DirectX::XMMATRIX Graphics::GetProjection() const
{
	return projection;
}

void Graphics::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.

	pDSV.Reset();
	pTarget.Reset();
	pSwap.Reset();
	pContext.Reset();
	pDevice.Reset();

	CreateDevice();

	CreateResources();
}

void Graphics::ReportLiveObjects()
{
	WRL::ComPtr<ID3D11Debug> pDebug;
	pDevice->QueryInterface(pDebug.GetAddressOf());
	pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
}

UINT Graphics::GetBackBufferWidth()
{
	return backBufferWidth;
}

UINT Graphics::GetBackBufferHeight()
{
	return backBufferHeight;
}

Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMessages) 
	:
	Exception(line, file),
	hr(hr)
{
	for (const auto& msg : infoMessages)
	{
		info += msg;
		info.push_back('\n');
	}

	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const
{
	std::ostringstream oss;
	oss << GetType() << " generated error code 0x" << std::hex << std::uppercase << GetErrorCode()
		<< " in " << GetOriginString() << std::endl;
	
	if (!info.empty())
	{
		oss << std::endl << "Details: " << std::endl << GetErrorInfo() << std::endl;
	}

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const
{
	return "Graphics HrException";
}

HRESULT Graphics::HrException::GetErrorCode() const
{
	return hr;
}

std::string Graphics::HrException::GetErrorInfo() const
{
	return info;
}

const char* Graphics::DeviceRemovedException::GetType() const
{
	return "Graphics Device Removed Exception (DXGI_ERROR_DEVICE_REMOVED)";
}
Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMessages)
	:
	Exception(line, file)
{
	for (const auto& message : infoMessages)
	{
		info += message;
		info.push_back('\n');
	}

	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const
{
	std::ostringstream oss;
	oss << GetType() << " in " << GetOriginString() << std::endl
		<< std::endl << "Details:" << std::endl
		<< GetErrorInfo() << std::endl;
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const
{
	return "Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const
{
	return info;
}
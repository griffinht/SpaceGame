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
	/*
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1; //aa
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2; //buffering
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//todo change to flip
	sd.Flags = 0;

	UINT creationFlags = 0;
#if defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr;
	GRAPHICS_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));

	WRL::ComPtr<ID3D11Resource> pBackBuffer;
	GRAPHICS_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	GRAPHICS_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));

	// create depth stensil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	
	(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	// bind depth state
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// create depth stensil texture
	WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 1280u;
	descDepth.Height = 720u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GRAPHICS_THROW_INFO(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	// create view of depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GRAPHICS_THROW_INFO(pDevice->CreateDepthStencilView(
		pDepthStencil.Get(), &descDSV, &pDSV
	));;

	// bind depth stensil view to OM
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());
	*/
	CreateDevice();
	CreateResources();
}

void Graphics::Present(UINT syncInterval, UINT flags)
{
	std::lock_guard<std::mutex> lockGuard(mutex);
	HRESULT hr;
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

	CD3D11_VIEWPORT viewport(0.0f, 0.0f, backBufferWidth, backBufferHeight);
	pContext->RSSetViewports(1, &viewport);
}

void Graphics::CreateDevice()
{
	std::lock_guard<std::mutex> lockGuard(mutex);
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

#ifndef NDEBUG
	WRL::ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(device.As(&d3dDebug)))
	{
		WRL::ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
		{
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// TODO: Add more message IDs here as needed.
			};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	GRAPHICS_THROW_INFO(device.As(&pDevice));
	GRAPHICS_THROW_INFO(context.As(&pContext));

	// TODO: Initialize device dependent objects here (independent of window size).
}

void Graphics::CreateResources()
{
	std::lock_guard<std::mutex> lockGuard(mutex);
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
}

void Graphics::SetFullscreenState(bool fullscreen)
{
	std::lock_guard<std::mutex> lockGuard(mutex);
	HRESULT hr;
	GRAPHICS_THROW_INFO(pSwap->SetFullscreenState(fullscreen, nullptr));
	ResizeBuffers(0, 0);
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
		
		pContext->OMGetRenderTargets(0, 0, 0);
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
	}
}

void Graphics::drawTriangle(float angle, float x, float z)
{
	std::lock_guard<std::mutex> lockGuard(mutex);
	HRESULT hr;

	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
	};

	// make buffer
	Vertex vertices[] =
	{
		{ -1.0f,-1.0f,-1.0f },
		{ 1.0f,-1.0f,-1.0f },
		{ -1.0f,1.0f,-1.0f },
		{ 1.0f,1.0f,-1.0f },
		{ -1.0f,-1.0f,1.0f },
		{ 1.0f,-1.0f,1.0f },
		{ -1.0f,1.0f,1.0f },
		{ 1.0f,1.0f,1.0f },
	};

	WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	
	GRAPHICS_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

	// bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// create index buffer
	const unsigned short indices[] =
	{
		0,2,1,2,3,1,
		1,3,5,3,7,5,
		2,6,3,3,6,7,
		4,5,7,4,7,6,
		0,4,2,2,4,6,
		0,1,4,1,5,4,

	};
	WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	GRAPHICS_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

	// bind index buffer
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// create constant buffer for transformation matrix
	struct ConstantBuffer
	{
		DX::XMMATRIX transform;
	};
	const ConstantBuffer cb =//todo replace with actual aspect ratio
	{
		{
			DX::XMMatrixTranspose(
				DX::XMMatrixRotationZ(angle) *
				DX::XMMatrixRotationX(angle) *
				DX::XMMatrixTranslation(x, 0.0f, z + 4.0f) *
				DX::XMMatrixPerspectiveLH(1.0f, 9.0f/16.0f, 0.5f, 10.0f)
			)
		}
	};
	WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	GRAPHICS_THROW_INFO(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	// bind constant buffer to vertex shader
	pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{1.0f,0.0f,1.0f},
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f},
			{1.0f,1.0f,0.0f},
			{0.0f,1.0f,1.0f}
		}
	};

	WRL::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &cb2;
	GRAPHICS_THROW_INFO(pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer));

	// bind constand buffer to pixel shader
	pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());

	// create pixel shader
	WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	WRL::ComPtr<ID3DBlob> pBlob;
	GRAPHICS_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	GRAPHICS_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	// bind pixel shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	// create and vertex shader
	WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	GRAPHICS_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	GRAPHICS_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
	// bind vertex shader
	pContext->VSSetShader(pVertexShader.Get(), 0, 0);

	// input
	WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	GRAPHICS_THROW_INFO(pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));

	// bind vertext layout
	pContext->IASetInputLayout(pInputLayout.Get());

	// set primitive to triangle list
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GRAPHICS_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}

void Graphics::OnDeviceLost()
{
	std::lock_guard<std::mutex> lockGuard(mutex);
	// TODO: Add Direct3D resource cleanup here.

	pDSV.Reset();
	pTarget.Reset();
	pSwap.Reset();
	pContext.Reset();
	pDevice.Reset();

	CreateDevice();

	CreateResources();
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
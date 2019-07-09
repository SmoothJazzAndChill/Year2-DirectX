#include "DeviceManager.h"

using namespace Microsoft::WRL;

DeviceManager::DeviceManager()
{
}


DeviceManager::~DeviceManager()
{
}

const D3D_FEATURE_LEVEL DeviceManager::levels[] = {
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_11_1
};

HRESULT DeviceManager::CreateDeviceResources(HWND hwnd)
{
	HRESULT hr = S_OK;

	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.Windowed = TRUE;
	desc.BufferCount = 2;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	desc.OutputWindow = hwnd;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	ComPtr<IDXGISwapChain> swapChain;

	hr = D3D11CreateDeviceAndSwapChain(
		nullptr, 
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, 
		nullptr, 
		deviceFlags, 
		levels, 
		ARRAYSIZE(levels), 
		D3D11_SDK_VERSION,
		&desc, 
		swapChain.GetAddressOf(), 
		device.GetAddressOf(), 
		&m_FeatureLevel, 
		context.GetAddressOf()
	);

	device.As(&m_pd3dDevice);
	context.As(&m_pd3dDeviceContext);
	swapChain.As(&m_pDXGISwapChain);

	hr = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer);
	m_pBackBuffer->GetDesc(&m_bbDesc);

	ZeroMemory(&m_Viewport, sizeof(D3D11_VIEWPORT));
	m_Viewport.Height = (float)m_bbDesc.Height;
	m_Viewport.Width = (float)m_bbDesc.Width;
	m_Viewport.MinDepth = 0;
	m_Viewport.MaxDepth = 1;

	m_pd3dDeviceContext->RSSetViewports(1, &m_Viewport);

	hr = m_pd3dDevice->CreateRenderTargetView(m_pBackBuffer.Get(), nullptr, m_pRenderTarget.GetAddressOf());

	return hr;
}

HRESULT DeviceManager::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags, levels, ARRAYSIZE(levels), D3D11_SDK_VERSION, &device, &m_FeatureLevel, &context);

	device.As(&m_pd3dDevice);
	context.As(&m_pd3dDeviceContext);

	return hr;
}

HRESULT DeviceManager::CreateWindowResources(HWND hwnd)
{
	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.Windowed = TRUE;
	desc.BufferCount = 2;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	desc.OutputWindow = hwnd;

	ComPtr<IDXGIDevice3> dxgiDevice;
	m_pd3dDevice.As(&dxgiDevice);

	ComPtr<IDXGIAdapter> adapter;
	ComPtr<IDXGIFactory> factory;

	hr = dxgiDevice->GetAdapter(&adapter);

	if (SUCCEEDED(hr))
	{
		adapter->GetParent(IID_PPV_ARGS(&factory));

		hr = factory->CreateSwapChain(m_pd3dDevice.Get(), &desc, &m_pDXGISwapChain);
	}

	hr = ConfigureBackBuffer();

	return hr;
}

HRESULT DeviceManager::ConfigureBackBuffer()
{
	HRESULT hr = S_OK;

	hr = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**) &m_pBackBuffer);

	hr = m_pd3dDevice->CreateRenderTargetView(m_pBackBuffer.Get(), nullptr, m_pRenderTarget.GetAddressOf());

	m_pBackBuffer->GetDesc(&m_bbDesc);

	CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT,static_cast<UINT> (m_bbDesc.Width), static_cast<UINT> (m_bbDesc.Height), 1, 1, D3D11_BIND_DEPTH_STENCIL);

	m_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencil);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

	m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil.Get(), &depthStencilViewDesc, &m_pDepthStencilView);

	ZeroMemory(&m_Viewport, sizeof(D3D11_VIEWPORT));
	m_Viewport.Height = (float)m_bbDesc.Height;
	m_Viewport.Width = (float)m_bbDesc.Width;
	m_Viewport.MinDepth = 0;
	m_Viewport.MaxDepth = 1;

	m_pd3dDeviceContext->RSSetViewports(1, &m_Viewport);

	return hr;
}

HRESULT DeviceManager::ReleaseBackBuffer()
{
	HRESULT hr = S_OK;

	m_pRenderTarget.Reset();
	m_pBackBuffer.Reset();
	m_pDepthStencilView.Reset();
	m_pDepthStencil.Reset();

	m_pd3dDeviceContext->Flush();

	return hr;
}

HRESULT DeviceManager::GoFullScreen()
{
	HRESULT hr = S_OK;

	hr = m_pDXGISwapChain->SetFullscreenState(TRUE, NULL);

	ReleaseBackBuffer();

	hr = m_pDXGISwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

	hr = ConfigureBackBuffer();

	return hr;
}

HRESULT DeviceManager::GoWindowed()
{
	HRESULT hr = S_OK;

	hr = m_pDXGISwapChain->SetFullscreenState(FALSE, NULL);

	ReleaseBackBuffer();

	hr = m_pDXGISwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

	hr = ConfigureBackBuffer();

	return hr;
}

float DeviceManager::GetAspectRatio()
{
	return static_cast<float> (m_bbDesc.Width) / static_cast<float> (m_bbDesc.Height);
}

void DeviceManager::Present()
{
	m_pDXGISwapChain->Present(1, 0);
}

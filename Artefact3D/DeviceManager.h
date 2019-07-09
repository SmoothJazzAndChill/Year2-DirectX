#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include <d3d11.h>
#include <wrl/client.h>
#include <dxgi.h>
#include <dxgi1_3.h>

class DeviceManager
{
public:
	DeviceManager();
	~DeviceManager();

	HRESULT CreateDeviceResources(HWND hWnd);
	HRESULT CreateDeviceResources();
	HRESULT CreateWindowResources(HWND hWnd);

	HRESULT ConfigureBackBuffer();
	HRESULT ReleaseBackBuffer();
	HRESULT GoFullScreen();
	HRESULT GoWindowed();

	float GetAspectRatio();

	ID3D11Device*			GetDevice()			{ return m_pd3dDevice.Get(); };
	ID3D11DeviceContext*	GetDeviceContext()	{ return m_pd3dDeviceContext.Get(); };
	ID3D11RenderTargetView*	GetRenderTarget()	{ return m_pRenderTarget.Get(); };
	ID3D11DepthStencilView* GetDepthStencil()	{ return m_pDepthStencilView.Get(); };

	void Present();

private:
	Microsoft::WRL::ComPtr<ID3D11Device>			m_pd3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_pd3dDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_pDXGISwapChain;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_pBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_pRenderTarget;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_pDepthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView;

	D3D_FEATURE_LEVEL		m_FeatureLevel;
	D3D11_TEXTURE2D_DESC	m_bbDesc;
	D3D11_VIEWPORT			m_Viewport;

	static const D3D_FEATURE_LEVEL levels[];
};


#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include <d3d11.h>
#include <vector>

#include <DirectXMath.h>

#include "DeviceManager.h"
#include "World.h"

#include "Transform.h"
#include "Camera.h"

#include "ModelLoader.h"
#include <DirectXTex\DirectXTex.h>

static std::string fVertexShaders[] =
{
	"vBasic",
	//"vTerrain"
};

static std::string fHullShaders[] =
{
	"hBasic",
	//"hTerrain"
};

static std::string fDomainShaders[] =
{
	"dBasic",
	//"dTerrain"
};

static std::string fPixelShaders[] =
{
	"fBasic",
	//"fTerrain"
};

static std::string fModels[] =
{
	//"Plane"
	"terrain",
	//"bb8",

	//Below won't load as they don't have texture coordinates
	//"Cube",
	//"Torus",
	//"Teapot",
	//"Sphere"
};

static std::string fTextures[] =
{
	"Terrain\\surface.png"
};

class RenderingManager
{
public:
	RenderingManager(std::shared_ptr<DeviceManager> deviceManager, std::shared_ptr<ModelLoader> loader, std::shared_ptr<World>* world);
	~RenderingManager();

	void CreateDeviceDependantResources();
	void CreateWindowSizeDependantResources();
	void Render();

private:
	HRESULT CreateShaders();
	void LoadModels();
	void LoadTextures();
	void CreateViewAndPerspective();

	std::shared_ptr<DeviceManager>	m_DeviceManager;
	std::shared_ptr<ModelLoader>	m_ModelLoader;
	std::shared_ptr<World>			m_World;

	std::shared_ptr<Entity>				Camera;
	std::shared_ptr<TransformComponent> CameraTransform;
	std::shared_ptr<CameraComponent>	CameraCamera;

	typedef struct _constantBufferStruct
	{
		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	} ConstantBufferStruct;

	typedef struct _cbProjection
	{
		DirectX::XMMATRIX Projection;
	} CBProjection;

	typedef struct _cbView
	{
		DirectX::XMMATRIX View;
		DirectX::XMFLOAT4 CameraPosition;
	} CBView;

	typedef struct _cbWorld
	{
		DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();;
	} CBWorld;

	static_assert((sizeof(ConstantBufferStruct) % 16) == 0, "Constant buffer size must be 16-byte aligned");

	ConstantBufferStruct m_constantBufferData;

	CBProjection	m_cbProjectionData;
	CBView			m_cbViewData;
	CBWorld			m_cbWorldData;

	unsigned int m_indexCount;
	unsigned int m_frameCount;

	DirectX::XMVECTOR eye;
	DirectX::XMVECTOR at;
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_pInputLayoutExtended;

	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_pCBProjection;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_pCBView;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_pCBWorld;
	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_pConstantBuffer;

	std::vector<std::shared_ptr<Model>>		Models;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>	Textures;
	std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> Samplers;

	std::vector<Microsoft::WRL::ComPtr<ID3D11VertexShader>> VertexShaders;
	std::vector<Microsoft::WRL::ComPtr<ID3D11HullShader>>	HullShaders;
	std::vector<Microsoft::WRL::ComPtr<ID3D11DomainShader>> DomainShaders;
	std::vector<Microsoft::WRL::ComPtr<ID3D11PixelShader>>	PixelShaders;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pPixelShader;
};


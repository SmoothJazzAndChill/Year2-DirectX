#include "RenderingManager.h"

#include <ppltasks.h>
#include <d3dcompiler.h>
#include <fstream>
#include <DirectXMath.h>
#include <DirectXTex\DirectXTex.h>


#define ASSETSFILEPATH "..\\Assets\\"

#if defined(DEBUG) || defined(_DEBUG)
#define SHADERPATH "..\\Debug\\"
#else
#define SHADERPATH "..\\Release\\"
#endif

RenderingManager::RenderingManager(std::shared_ptr<DeviceManager> deviceManager, std::shared_ptr<ModelLoader> loader, std::shared_ptr<World>* world) : m_frameCount(0), m_DeviceManager(deviceManager), m_ModelLoader(loader)
{
	m_frameCount = 0;

	m_World = *world;

	m_cbViewData.CameraPosition = { 0.0f, 20.0f, -30.0f, 0.0f };
}

RenderingManager::~RenderingManager()
{
}


HRESULT RenderingManager::CreateShaders()
{
	HRESULT hr = S_OK;

	ID3D11Device* Device = m_DeviceManager->GetDevice();

	FILE* Shader;
	BYTE* bytes;
	size_t destSize = 32000;
	size_t bytesRead;

	// Vertex Shaders
	for (int i = 0; i < ARRAYSIZE(fVertexShaders); i++)
	{
		bytes = new BYTE[destSize];

		bytesRead = 0;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pShader;
		fopen_s(&Shader, (SHADERPATH + fVertexShaders[i] + ".cso").c_str(), "rb");
		bytesRead = fread_s(bytes, destSize, 1, 32000, Shader);
		hr = Device->CreateVertexShader(
			bytes,
			bytesRead,
			nullptr,
			&pShader
		);

		D3D11_INPUT_ELEMENT_DESC iaDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
			0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		};

		hr = Device->CreateInputLayout(
			iaDesc,
			ARRAYSIZE(iaDesc),
			bytes,
			bytesRead,
			&m_pInputLayout
		);

		VertexShaders.push_back(pShader);

		delete bytes;
		fclose(Shader);
		Shader = nullptr;
	}

	// Hull Shaders
	for (int i = 0; i < ARRAYSIZE(fHullShaders); i++)
	{
		bytes = new BYTE[destSize];
		bytesRead = 0;
		Microsoft::WRL::ComPtr<ID3D11HullShader> pShader;

		fopen_s(&Shader, (SHADERPATH + fHullShaders[i] + ".cso").c_str(), "rb");
		bytesRead = fread_s(bytes, destSize, 1, 32000, Shader);
		hr = Device->CreateHullShader(
			bytes,
			bytesRead,
			nullptr,
			pShader.GetAddressOf()
		);

		HullShaders.push_back(pShader);

		delete bytes;
		fclose(Shader);
		Shader = nullptr;
	}

	// Domain Shaders
	for (int i = 0; i < ARRAYSIZE(fDomainShaders); i++)
	{
		bytes = new BYTE[destSize];
		bytesRead = 0;
		Microsoft::WRL::ComPtr<ID3D11DomainShader> pShader;

		fopen_s(&Shader, (SHADERPATH + fDomainShaders[i] + ".cso").c_str(), "rb");
		bytesRead = fread_s(bytes, destSize, 1, 32000, Shader);
		hr = Device->CreateDomainShader(
			bytes,
			bytesRead,
			nullptr,
			pShader.GetAddressOf()
		);

		DomainShaders.push_back(pShader);

		delete bytes;
		fclose(Shader);
		Shader = nullptr;
	}

	// Pixel Shaders
	for (int i = 0; i < ARRAYSIZE(fPixelShaders); i++)
	{
		bytes = new BYTE[destSize];
		bytesRead = 0;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pShader;

		fopen_s(&Shader, (SHADERPATH + fPixelShaders[i] + ".cso").c_str(), "rb");
		bytesRead = fread_s(bytes, destSize, 1, 32000, Shader);
		hr = Device->CreatePixelShader(
			bytes,
			bytesRead,
			nullptr,
			pShader.GetAddressOf()
		);

		PixelShaders.push_back(pShader);

		delete bytes;
		fclose(Shader);
		Shader = nullptr;
	}

	// Const Buffer Stuff
	CD3D11_BUFFER_DESC cbPDesc(
		sizeof(CBProjection),
		D3D11_BIND_CONSTANT_BUFFER
	);

	hr = Device->CreateBuffer(
		&cbPDesc,
		nullptr,
		m_pCBProjection.GetAddressOf()
	);

	CD3D11_BUFFER_DESC cbVDesc(
		sizeof(CBView),
		D3D11_BIND_CONSTANT_BUFFER
	);

	hr = Device->CreateBuffer(
		&cbVDesc,
		nullptr,
		m_pCBView.GetAddressOf()
	);

	CD3D11_BUFFER_DESC cbWDesc(
		sizeof(CBWorld),
		D3D11_BIND_CONSTANT_BUFFER
	);

	hr = Device->CreateBuffer(
		&cbWDesc,
		nullptr,
		m_pCBWorld.GetAddressOf()
	);
	
	return hr;
}

void RenderingManager::LoadModels()
{
	std::shared_ptr<Model> M;

	for (std::string s : fModels)
	{
		OutputDebugString(("Loading " + s + ".obj\n").c_str());
		M = m_ModelLoader->LoadModel(ASSETSFILEPATH + s + ".obj");
		Models.push_back(M);
	}
}

void RenderingManager::LoadTextures()
{
	ID3D11ShaderResourceView* Resource = nullptr;
	for (std::string s : fTextures)
	{
		DirectX::ScratchImage image;
		std::string Full = ASSETSFILEPATH + s;
		std::wstring Path = std::wstring(Full.begin(), Full.end());
		DirectX::LoadFromWICFile(Path.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image);
		DirectX::CreateShaderResourceView(m_DeviceManager->GetDevice(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &Resource);

		Textures.push_back(Resource);
	}

	ID3D11SamplerState* Sampler = nullptr;

	D3D11_SAMPLER_DESC desc;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = 0;

	m_DeviceManager->GetDevice()->CreateSamplerState(&desc, &Sampler);
	Samplers.push_back(Sampler);
}

void RenderingManager::CreateDeviceDependantResources()
{
	CreateShaders();
	LoadTextures();
	LoadModels();
	/*auto CreateShadersTask = Concurrency::create_task(
		[this]()
		{
			CreateShaders();
		}
	);

	auto CreateCubeTask = CreateShadersTask.then(
		[this]()
		{
			CreateCube();
		}
	);

	auto LoadModelsTask = CreateShadersTask.then(
		[this]()
		{
			LoadModels();
		}
	);*/
}

void RenderingManager::CreateWindowSizeDependantResources()
{
	CreateViewAndPerspective();
}

void RenderingManager::Render()
{
	unsigned int ShaderSet = 0;
	unsigned int ModelNumber = 0;

	m_pVertexBuffer = Models[ModelNumber]->VertexBuffer;
	m_pIndexBuffer = Models[ModelNumber]->IndexBuffer;
	m_indexCount = Models[ModelNumber]->IndexCount;

	ID3D11DeviceContext* context = m_DeviceManager->GetDeviceContext();

	ID3D11RenderTargetView* renderTarget = m_DeviceManager->GetRenderTarget();
	ID3D11DepthStencilView* depthStencil = m_DeviceManager->GetDepthStencil();

	m_cbWorldData.World = DirectX::XMMatrixIdentity();

	eye = DirectX::XMLoadFloat3(&CameraTransform->Position.GetDX());
	at = DirectX::XMLoadFloat3(&CameraCamera->Target.GetDX());
	DirectX::XMStoreFloat4(&m_cbViewData.CameraPosition, eye);

	m_cbViewData.View = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(eye, at, up));

	context->UpdateSubresource(m_pCBProjection.Get(), 0, nullptr, &m_cbProjectionData, 0, 0);
	context->UpdateSubresource(m_pCBView.Get(), 0, nullptr, &m_cbViewData, 0, 0);
	context->UpdateSubresource(m_pCBWorld.Get(), 0, nullptr, &m_cbWorldData, 0, 0);

	const float ClearColour[] = { 0.53f, 0.81f, 0.92f, 1.000f };
	context->ClearRenderTargetView(renderTarget, ClearColour);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	UINT stride = sizeof(ModelLoader::VertexPositionColour);
	UINT offset = 0;

	context->IASetVertexBuffers(
		0,
		1,
		m_pVertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	context->IASetIndexBuffer(
		m_pIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);

	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);

	context->IASetInputLayout(m_pInputLayout.Get());

	// Vertex Shader Setup
#pragma region VertexShader

	context->VSSetShader(
		VertexShaders[ShaderSet].Get(),
		nullptr,
		0
	);

	context->VSSetConstantBuffers(
		0,
		1,
		m_pCBProjection.GetAddressOf()
	);

	context->VSSetConstantBuffers(
		1,
		1,
		m_pCBView.GetAddressOf()
	);

	context->VSSetConstantBuffers(
		2,
		1,
		m_pCBWorld.GetAddressOf()
	);
#pragma endregion
	/*
	// Hull Shader Setup
#pragma region HullShader
	context->HSGetShader(
		HullShaders[ShaderSet].GetAddressOf(),
		nullptr,
		0
	);
#pragma endregion

	// Domain Shader Setup
#pragma region DomainShader
	context->DSGetShader(
		DomainShaders[ShaderSet].GetAddressOf(),
		nullptr,
		0
	);
#pragma endregion
*/
	// Pixel Shader Setup
#pragma region PixelShader
	context->PSSetShader(
		PixelShaders[ShaderSet].Get(),
		nullptr,
		0
	);

	context->PSSetSamplers(
		0,
		1,
		Samplers[0].GetAddressOf()
	);

	context->PSSetShaderResources(
		0,
		1,
		Textures[0].GetAddressOf()
	);

	context->PSSetConstantBuffers(
		0,
		1,
		m_pCBView.GetAddressOf()
	);
#pragma endregion

	context->DrawIndexed(
		m_indexCount,
		0,
		0
	);
}

void RenderingManager::CreateViewAndPerspective()
{
	Camera = m_World->Find(CAMERA);
	CameraTransform = Camera->GetComponent<TransformComponent>();
	CameraCamera = Camera->GetComponent<CameraComponent>();

	eye = DirectX::XMLoadFloat3(&CameraTransform->Position.GetDX());
	at = DirectX::XMLoadFloat3(&CameraCamera->Target.GetDX());

	m_cbViewData.View = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(eye,at,up));

	float aspectRatio = m_DeviceManager->GetAspectRatio();

	m_cbProjectionData.Projection = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovRH(DirectX::XMConvertToRadians(90), aspectRatio, 0.01f, 10000.0f));
}
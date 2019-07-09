#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <wrl/client.h>
#include <fstream>
#include <vector>

#include "DeviceManager.h"

struct Model
{
public:
	unsigned int IndexCount;
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
};

class ModelLoader
{
public:
	typedef unsigned long Index;

	typedef struct _vertexPositionColour
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 normal;
		DirectX::XMFLOAT4 texcoord;
	} VertexPositionColour;

	static_assert((sizeof(VertexPositionColour) % 16) == 0, "Constant buffer size must be 16-byte aligned");

	ModelLoader(std::shared_ptr<DeviceManager> Manager);
	~ModelLoader();

	std::shared_ptr<Model> LoadModel(std::string FileName);

private:
	std::shared_ptr<DeviceManager> m_DeviceManager;
	DirectX::XMFLOAT4 CalculateNormals(unsigned int vIndex, std::vector<std::vector<Index>>* IndexMap, std::vector<Index>* NormalsIndices, std::vector<DirectX::XMFLOAT3>* Normals);
	DirectX::XMFLOAT4 GetTextureCoordinates(unsigned int vIndex, std::vector<std::vector<Index>>* IndexMap, std::vector<Index>* CoordinatesIndices, std::vector<DirectX::XMFLOAT2>* TextureCoordinates);
};


#include "ModelLoader.h"

#include <sstream>
#include <math.h>
#include <string>

using namespace std;

ModelLoader::ModelLoader(shared_ptr<DeviceManager> Manager) : m_DeviceManager(Manager)
{
}

ModelLoader::~ModelLoader()
{
}

std::shared_ptr<Model> ModelLoader::LoadModel(std::string FileName)
{
	ID3D11Device* device = m_DeviceManager->GetDevice();

	// Create a new model
	shared_ptr<Model> _model = shared_ptr<Model>(new Model());

	fstream Model;
	string Line;
	string Token;

	// Store all of the data. Using free memory because a lot of data might be loaded
	vector<float>* Points = new vector<float>();
	vector<float>* Normals = new vector<float>();
	vector<float>* CoordinatePoints = new vector<float>();
	vector<Index>* Indices = new vector<Index>();
	vector<Index>* NormalsIndices = new vector<Index>();
	vector<Index>* CoordinateIndices = new vector<Index>();

	vector<DirectX::XMFLOAT3>* NormalsVectors = new vector<DirectX::XMFLOAT3>();
	vector<VertexPositionColour>* Vertices = new vector<VertexPositionColour>();
	vector<DirectX::XMFLOAT2>* Coordinates = new vector<DirectX::XMFLOAT2>();
	
	vector<vector<Index>>* IndexMap = new vector<vector<Index>>();

	// Open the obj file to be read
	Model.open(FileName);

	OutputDebugString("Reading File\n");

	// Read file
	/*
		
	*/
	while (getline(Model, Line, '\n'))
	{
		if (Line[0] == '#' || Line[0] == '\n') continue;

		if (Line.find("v ") != string::npos)
		{
			stringstream lineStream(Line);
			while (getline(lineStream, Token, ' '))
			{
				if (Token == "v" || Token == "") continue;

				Points->push_back(stof(Token.substr(0, Token.find("\n"))));
			}
		}

		if (Line.find("vn ") != string::npos)
		{
			stringstream lineStream(Line);
			while (getline(lineStream, Token, ' '))
			{
				if (Token == "vn" || Token == "") continue;

				Normals->push_back(stof(Token.substr(0, Token.find("\n"))));
			}
		}

		if (Line.find("vt ") != string::npos)
		{
			stringstream lineStream(Line);
			while (getline(lineStream, Token, ' '))
			{
				if (Token == "vt" || Token == "") continue;

				CoordinatePoints->push_back(stof(Token.substr(0, Token.find("\n"))));
			}
		}

		if (Line.find("f ") != string::npos)
		{
			stringstream lineStream(Line);
			while (getline(lineStream, Token, ' '))
			{
				if (Token == "f" || Token == "") continue;

				Indices->push_back(stoi(Token.substr(0, Token.find("/"))));
				std::string rem = Token.substr(Token.find("/") + 1, Token.find("\n"));
				CoordinateIndices->push_back(stoi(rem.substr(0, rem.find("/"))));
				rem = rem.substr(rem.find("/") + 1, rem.find("\n"));
				NormalsIndices->push_back(stoi(rem));

			}
		}
	}

	// Close File
	Model.close();

	// Set Index Map size
	IndexMap->resize(Points->size() / 3);

	OutputDebugString("Mapping Indices\n");

	// Minus one from all indices and map them
	for (unsigned int i = 0; i < Indices->size(); i++)
	{
		(*Indices)[i]--;
		(*NormalsIndices)[i]--;
		(*CoordinateIndices)[i]--;

		// For each of the different indices tell me where to find them
		((*IndexMap)[(*Indices)[i]]).push_back(i);
	}

	OutputDebugString("Converting Normals\n");

	// Convert Normals to a Vector (mathematical vector)
	for (unsigned int i = 0; i < Normals->size(); i += 3)
	{
		NormalsVectors->push_back({ (*Normals)[i], (*Normals)[i+1], (*Normals)[i+2] });
	}

	for (unsigned int i = 0; i < CoordinatePoints->size(); i += 3)
	{
		Coordinates->push_back({ (*CoordinatePoints)[i], (*CoordinatePoints)[i + 1] });
	}

	OutputDebugString("Converting Points\n");

	// Convert points into vertices
	unsigned int v = 0;
	for (unsigned int i = 0; i < Points->size(); i += 3)
	{
		VertexPositionColour Vertex;
		Vertex.pos = { (*Points)[i], (*Points)[i + 1], (*Points)[i + 2], 1.0f};
		Vertex.normal = CalculateNormals(v, IndexMap, NormalsIndices, NormalsVectors);
		Vertex.texcoord = GetTextureCoordinates(v, IndexMap, CoordinateIndices, Coordinates);
		Vertices->push_back(Vertex);

		v++;
	}

	// Create the vertex buffer
	CD3D11_BUFFER_DESC vDesc(
		Vertices->size() * sizeof(VertexPositionColour),
		D3D11_BIND_VERTEX_BUFFER
	);

	D3D11_SUBRESOURCE_DATA vData;
	ZeroMemory(&vData, sizeof(D3D11_SUBRESOURCE_DATA));
	vData.pSysMem = &((*Vertices)[0]);
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;

	device->CreateBuffer(
		&vDesc,
		&vData,
		&_model->VertexBuffer
	);

	OutputDebugString("Reversing Indices\n");

	// Reverse Indices because of back-face culling
	vector<Index>* rIndices = new vector<Index>();
	for (unsigned int i = Indices->size(); i > 0; i--)
	{
		rIndices->push_back((*Indices)[i - 1]);
	}

	// Set Index count for the model 
	_model->IndexCount = Indices->size();

	// Create Index Buffer
	CD3D11_BUFFER_DESC iDesc(
		rIndices->size() * sizeof(Index),
		D3D11_BIND_INDEX_BUFFER
	);

	D3D11_SUBRESOURCE_DATA iData;
	ZeroMemory(&iData, sizeof(D3D11_SUBRESOURCE_DATA));
	iData.pSysMem = &((*rIndices)[0]);
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;
	
	device->CreateBuffer(
		&iDesc,
		&iData,
		&_model->IndexBuffer
	);

	// Cleanup
	delete Points;
	Points = nullptr;

	delete Vertices;
	Vertices = nullptr;

	delete Indices;
	Indices = nullptr;

	delete Normals;
	Normals = nullptr;

	delete NormalsVectors;
	NormalsVectors = nullptr;

	delete Coordinates;
	Coordinates = nullptr;

	delete CoordinateIndices;
	CoordinateIndices = nullptr;

	delete CoordinatePoints;
	CoordinatePoints = nullptr;

	delete IndexMap;
	IndexMap = nullptr;

	delete rIndices;
	rIndices = nullptr;

	OutputDebugString(("Loaded: " + FileName + "\n").c_str());

	// Return the model
	return _model;
}

DirectX::XMFLOAT4 ModelLoader::CalculateNormals(unsigned int vIndex, vector<vector<Index>>* IndexMap, vector<Index>* NormalsIndices, vector<DirectX::XMFLOAT3>* Normals)
{
	vector<DirectX::XMFLOAT3> ReferencedNormals;

	DirectX::XMFLOAT3 vNormal = { 0,0,0 };
	float vNormalMagnitude;

	// Get the normals
	for (unsigned int n : (*IndexMap)[vIndex])
	{
		ReferencedNormals.push_back((*Normals)[(*NormalsIndices)[n]]);
	}

	// Add them together
	for (DirectX::XMFLOAT3 n : ReferencedNormals)
	{
		vNormal.x += n.x;
		vNormal.y += n.y;
		vNormal.z += n.z;
	}

	// Calculate the magnitude
	vNormalMagnitude = sqrt((vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z));

	// Normalize vector
	vNormal = { vNormal.x / vNormalMagnitude, vNormal.y / vNormalMagnitude, vNormal.z / vNormalMagnitude };

	DirectX::XMFLOAT4 result = { 0,0,0,0 };

	result.x = vNormal.x;
	result.y = vNormal.y;
	result.z = vNormal.z;

	return result;
}

DirectX::XMFLOAT4 ModelLoader::GetTextureCoordinates(unsigned int vIndex, vector<vector<Index>>* IndexMap, vector<Index>* CoordinatesIndices, vector<DirectX::XMFLOAT2>* TextureCoordinates)
{
	DirectX::XMFLOAT2 c = { 0,0 };
	DirectX::XMFLOAT4 Coordinate = { 0,0,0,0 };

	for (unsigned int n : (*IndexMap)[vIndex])
	{
		c = ((*TextureCoordinates)[(*CoordinatesIndices)[n]]);
	}

	Coordinate.x = c.x;
	Coordinate.y = c.y;

	return Coordinate;
}
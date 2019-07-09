#pragma once
#include <DirectXMath.h>

#include "World.h"
#include "Entity.h"

class World;
class Entity;

class System
{
public:
	std::shared_ptr<World> GameWorld;

	typedef DirectX::XMFLOAT2 float2;
	typedef DirectX::XMFLOAT3 float3;
	typedef DirectX::XMFLOAT4 float4;

	// Virtual functions to be overriden by derived systems
	virtual void Start() {}
	virtual void Update(){}
	virtual void FixedUpdate(){}
};


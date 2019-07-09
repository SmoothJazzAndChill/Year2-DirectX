#pragma once
#include <d3d11.h>

#include "Component.h"
class Material :
	public Component
{
public:
	UINT32 Sampler;

	UINT32 Diffuse;

};


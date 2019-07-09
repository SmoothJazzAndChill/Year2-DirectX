#pragma once
#include "Component.h"

class RenderingComponent :
	public Component
{
public:
	UINT32 VertexShader = 0;
	UINT32 HullShader = 0;
	UINT32 DomainShader = 0;
	UINT32 PixelShader = 0;
	
	UINT32 Model;

};


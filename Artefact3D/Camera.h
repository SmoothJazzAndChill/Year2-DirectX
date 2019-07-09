#pragma once

#include <DirectXMath.h>

#include "Vectors.h"
/*
	#Camera Component System#
	Stores camera details that are passed to the renderer
*/
#include "Component.h"
class CameraComponent :
	public Component
{
public:
	Vector3 StartPosition = { 0.0f, 0.0f, -30.0f };
	Vector3 Target = { 0.0f, 0.0f, 1.0f };
};


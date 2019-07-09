#pragma once
#include <DirectXMath.h>

#include "Component.h"
#include "Vectors.h"


class TransformComponent :
	public Component
{
public:
	Vector3 Position = { 0.0f, 0.0f, 0.0f };
	Vector3 Rotation = { 0.0f, 0.0f, 0.0f };
	Vector3 Scale = { 0.0f, 0.0f, 0.0f };

	Vector3 Forwards()
	{
		Vector3 Forwards = { 0.0f, 0.0f, 1.0f };

		DirectX::XMFLOAT3 f;

		DirectX::XMStoreFloat3(&f, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&Forwards.GetDX()), DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&Rotation.GetDX()))));

		Forwards = { f.x, f.y, f.z };

		return Forwards;
	}
	Vector3 Left()
	{
		Vector3 Left = { 1.0f, 0.0f, 0.0f };

		DirectX::XMFLOAT3 l;

		DirectX::XMStoreFloat3(&l, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&Left.GetDX()), DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&Rotation.GetDX()))));

		Left = { l.x, l.y, l.z };

		return Left;
	}
	Vector3 Top()
	{
		Vector3 Top = { 0.0f, 1.0f, 0.0f };

		DirectX::XMFLOAT3 t;

		DirectX::XMStoreFloat3(&t, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&Top.GetDX()), DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&Rotation.GetDX()))));

		Top = { t.x, t.y, t.z };

		return Top;
	}
	Vector3 Up = { 0.0f, 1.0f, 0.0f };

	void Translate(Vector3 Direction)
	{
		Position.x += Direction.x;
		Position.y += Direction.y;
		Position.z += Direction.z;
	}
};


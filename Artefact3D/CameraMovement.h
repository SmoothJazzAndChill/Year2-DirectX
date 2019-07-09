#pragma once
#include "Transform.h"
#include "Camera.h"
#include "Input.h"

/*
	#Camera Movement System#
	Handles moving the camera aroud
*/

void OutputVector(Vector3 v);
void OutputVector(Vector2 v);

#include "System.h"
class CameraMovementSystem :
	public System
{
public:
	pEntity Camera;
	std::shared_ptr<TransformComponent> CameraTransform;
	std::shared_ptr<CameraComponent> CameraCamera;
	std::shared_ptr<InputComponent> Input;

	Vector3 TargetRotation;
	Vector3 CurrentRotation;
	Vector3 RotationDirection;
	float MovementSpeed = 0.5f;

	float Sensitivity = 0.001f;

	void Start()
	{
		Camera = GameWorld->Find(CAMERA);
		CameraTransform = Camera->GetComponent<TransformComponent>();
		CameraCamera = Camera->GetComponent<CameraComponent>();
		Input = GameWorld->Find(GAMEMANAGER)->GetComponent<InputComponent>();

		CameraTransform->Position = CameraCamera->StartPosition;
		CameraCamera->Target = CameraTransform->Forwards();
	}

	void Update()
	{
		CurrentRotation = CameraTransform->Rotation;
		TargetRotation = CurrentRotation;
		RotationDirection = { 0, 0, 0 };

		for (unsigned int i = 0; i < Input->MouseMovements.size(); i++)
		{
			Vector3 Rotation = { Input->MouseY(), Input->MouseX() * -1, 0 };

			TargetRotation = TargetRotation + Rotation;
			RotationDirection = TargetRotation - CurrentRotation;
			RotationDirection.Normalize();

			// RotationDirection can break sometimes, this prevents the camera from screwing up
			if (std::to_string(RotationDirection.x) == "-nan(ind)")
			{
				OutputDebugString("Something broke!\n");
				OutputDebugString("=================\n");
				RotationDirection.x = 0;
			}
			if (std::to_string(RotationDirection.y) == "-nan(ind)")
			{
				RotationDirection.y = 0;
			}

			Input->MouseMovements.pop();
		}

		CameraTransform->Rotation = CurrentRotation + (RotationDirection * Sensitivity);

		if (CameraTransform->Rotation.x > 89.0f) CameraTransform->Rotation.x = 89.0f;
		if (CameraTransform->Rotation.x < -89.0f) CameraTransform->Rotation.x = -89.0f;

		if (Input->GetKey(KEY_W))
		{
			// Move Forwards
			CameraTransform->Translate(CameraTransform->Forwards() * MovementSpeed);
		}

		if (Input->GetKey(KEY_S))
		{
			// Move Backwards
			CameraTransform->Translate(CameraTransform->Forwards() * -MovementSpeed);
		}

		if (Input->GetKey(KEY_A))
		{
			// Move Left
			CameraTransform->Translate(CameraTransform->Left() * MovementSpeed);
		}

		if (Input->GetKey(KEY_D))
		{
			// Move Right
			CameraTransform->Translate(CameraTransform->Left() * -MovementSpeed);
		}

		if (Input->GetKey(KEY_SPACE))
		{
			// Move Backwards
			CameraTransform->Translate(CameraTransform->Up * MovementSpeed);
		}

		if (Input->GetKey(KEY_SHIFT))
		{
			// Move Backwards
			CameraTransform->Translate(CameraTransform->Up * -MovementSpeed);
		}

		CameraCamera->Target = CameraTransform->Position + CameraTransform->Forwards();
	}

};

void OutputVector(Vector3 v)
{
	OutputDebugString((std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + "\n" ).c_str());
}

void OutputVector(Vector2 v)
{
	Vector3 _v = { v, 0 };
	OutputVector(_v);
}


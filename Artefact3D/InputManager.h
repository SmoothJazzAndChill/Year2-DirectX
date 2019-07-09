#pragma once
#include <Windowsx.h>

#include "Vectors.h"
#include "Input.h"

/*
	#Input Manager System#
	Handles input from the system and organises it
*/
#include "System.h"
class InputManagerSystem :
	public System
{
public:
	pEntity GameManager;
	std::shared_ptr<InputComponent> input;

	POINT Middle;
	POINT* m = &Middle;
	InputComponent::MouseMovement NewMovement;

	void Start()
	{
		GameManager = GameWorld->Find(GAMEMANAGER);
		input = GameManager->GetComponent<InputComponent>();
		
		GetCursorPos(m);

		OutputDebugString((std::to_string(Middle.x) + ", " + std::to_string(Middle.y) + "\n").c_str());
	}

	void Update()
	{
		input->RaisedKeys.clear();
		input->PressedKeys.clear();

		// Clear Mouse Movements Queue frames if there are any remaining from the Middle frame
		for (unsigned int i = 0; i < input->MouseMovements.size(); i++)
		{
			input->MouseMovements.pop();
		}

		for (unsigned int i = 0; i < input->InputQueue.size(); i++)
		{
			MSG* msg = &input->InputQueue.front();

			// Mouse Movement
			if (msg->message == WM_MOUSEMOVE)
			{
				long x = GET_X_LPARAM(msg->lParam);
				long y = GET_Y_LPARAM(msg->lParam);

				NewMovement.MouseX = x - m->x;
				NewMovement.MouseY = y - m->y;

				OutputDebugString((std::to_string(x) + ", " + std::to_string(y) + "\n").c_str());

				input->MouseMovements.push(NewMovement);
			}

			// Keyboard Input
			if (msg->message == WM_KEYDOWN)
			{
				if (!input->GetKey(msg->wParam))
				{
					input->PressedKeys.insert(msg->wParam);
					input->HeldKeys.insert(msg->wParam);
				}
			}

			if (msg->message == WM_KEYUP)
			{
				input->HeldKeys.erase(msg->wParam);
				input->RaisedKeys.insert(msg->wParam);
			}

			
			input->InputQueue.pop();
		}
	}
};


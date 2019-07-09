#pragma once
#include <queue>
#include <set>
#include <Windows.h>

/*
	#Input Component#
	Stores the data needed by the input manager system and the resulting organised data
*/
#include "Component.h"

enum Keys
{
	KEY_A = 0x41, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
	KEY_SPACE = VK_SPACE,
	KEY_ENTER = VK_RETURN,
	KEY_SHIFT = VK_SHIFT,
	KEY_ESCAPE = VK_ESCAPE,
	KEY_CONTROL = VK_CONTROL
};

class InputComponent :
	public Component
{
public:
	struct MouseMovement
	{
		long MouseX;
		long MouseY;
	};

	std::queue<MSG> InputQueue;

	std::queue<MouseMovement> MouseMovements;

	std::set<UINT> PressedKeys;
	std::set<UINT> HeldKeys;
	std::set<UINT> RaisedKeys;

	float MouseX()
	{
		return (float)MouseMovements.front().MouseX;
	}

	float MouseY()
	{
		return (float)MouseMovements.front().MouseY;
	}

	bool GetKey(UINT Key)
	{
		if (HeldKeys.find(Key) != HeldKeys.end()) return true;
		else return false;
	}

	bool GetKeyDown(UINT Key)
	{
		if (PressedKeys.find(Key) != PressedKeys.end()) return true;
		else return false;
	}

	bool GetKeyUp(UINT Key)
	{
		if (RaisedKeys.find(Key) != RaisedKeys.end()) return true;
		else return false;
	}
	
};


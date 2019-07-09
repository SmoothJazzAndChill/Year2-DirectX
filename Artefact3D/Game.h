#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include <d3d11.h>

#include "DeviceManager.h"
#include "RenderingManager.h"

#include "World.h"

class Game
{
public:
	Game();
	~Game();

	HRESULT CreateDesktopWindow();

	HWND GetWindowHandle() { return m_hWnd; }

	static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HRESULT Run(std::shared_ptr<DeviceManager> deviceManager, std::shared_ptr<RenderingManager> renderingManager, std::shared_ptr<World> world);

private:
	HMENU m_hMenu;
	RECT m_rc;
	HWND m_hWnd;
};

static HINSTANCE m_hInstance;
static std::wstring m_WindowClassName;


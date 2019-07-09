#include "Game.h"

#include <string>
#include <Windowsx.h>

Game::Game()
{
	m_WindowClassName = L"GameWindowClass";
	m_hInstance = NULL;
}

Game::~Game()
{
}

HRESULT Game::CreateDesktopWindow()
{
	// Instance declaration
	if (m_hInstance == NULL)
	{
		m_hInstance = (HINSTANCE)GetModuleHandle(NULL);
	}

	// Icon Setup
	HICON hIcon = NULL;
	WCHAR szExePath[MAX_PATH];
	GetModuleFileName(NULL, (LPSTR)szExePath, MAX_PATH);

	if (hIcon == NULL)
	{
		hIcon = ExtractIcon(m_hInstance, (LPSTR)szExePath, 0);
	}

	// Window Setup
	WNDCLASS wndClass;
	wndClass.style = CS_DBLCLKS;
	wndClass.lpfnWndProc = Game::StaticWindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = hIcon;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = (LPCSTR)m_WindowClassName.c_str();

	if (!RegisterClass(&wndClass))
	{
		DWORD dwError = GetLastError();
		if (dwError != ERROR_CLASS_ALREADY_EXISTS)
		{
			return HRESULT_FROM_WIN32(dwError);
		}
	}

	// Setup Window Rect
	int x = CW_USEDEFAULT;
	int y = CW_USEDEFAULT;

	m_hMenu = NULL;

	int nDefaultWidth = 1366;
	int nDefaultHeight = 768;
	SetRect(&m_rc, 0, 0, nDefaultWidth, nDefaultHeight);
	AdjustWindowRect(&m_rc, WS_OVERLAPPEDWINDOW, (m_hMenu != NULL) ? true : false);

	m_hWnd = CreateWindow((LPCSTR)m_WindowClassName.c_str(), (LPCSTR)"Game", WS_OVERLAPPEDWINDOW, x, y, (m_rc.right - m_rc.left), (m_rc.bottom - m_rc.top), 0, m_hMenu, m_hInstance, 0);

	if (m_hWnd == NULL)
	{
		DWORD dwError = GetLastError();
		return HRESULT_FROM_WIN32(dwError);
	}

	return S_OK;
}

HRESULT Game::Run(std::shared_ptr<DeviceManager> deviceManager, std::shared_ptr<RenderingManager> renderingManager, std::shared_ptr<World> world)
{
	HRESULT hr = S_OK;

	if (!IsWindowVisible(m_hWnd))
	{
		ShowWindow(m_hWnd, SW_SHOW);
	}

	RECT rcClip;

	GetWindowRect(m_hWnd, &rcClip);

	HCURSOR Cursor = GetCursor();

	long WindowMidX = rcClip.right / 2;
	long WindowMidY = rcClip.bottom / 2;

	POINT vCursorPos;
	POINT* CursorPos = &vCursorPos;
	CursorPos->x = 0;
	CursorPos->y = 0;
	bool EnableCursor = false;

	SetCursorPos(WindowMidX, WindowMidY);
	ShowCursor(false);
	bool Success = ClipCursor(&rcClip);

	GetCursorPos(CursorPos);
	//WindowMidX = CursorPos->x;
	//WindowMidY = CursorPos->y;

	world->Setup();

	OutputDebugString((std::to_string(CursorPos->x) + ", " + std::to_string(CursorPos->y) + "\n").c_str());

	bool bGotMsg;
	MSG msg;
	msg.message = WM_NULL;
	bool mset = false;

	while (WM_QUIT != msg.message)
	{
		bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

		if (bGotMsg)
		{
			if (msg.message == WM_MOUSEMOVE)
			{
				if (EnableCursor)
				{
					world->QueueInput(&msg);
				}

				GetCursorPos(CursorPos);
				if (CursorPos->x == WindowMidX && CursorPos->y == WindowMidY)
				{
					EnableCursor = true;
				}
				else
				{
					EnableCursor = false;
				}

			}
			else
			{
				world->QueueInput(&msg);
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			world->Update();
			renderingManager->Render();
			deviceManager->Present();
		}

		SetCursorPos(WindowMidX, WindowMidY);
	}

	ShowCursor(true);
	ClipCursor(NULL);

	return hr;
}

LRESULT CALLBACK Game::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		HMENU hMenu;
		hMenu = GetMenu(hWnd);
		if (hMenu != NULL)
		{
			DestroyMenu(hMenu);
		}
		DestroyWindow(hWnd);
		UnregisterClass((LPCSTR)m_WindowClassName.c_str(), m_hInstance);
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

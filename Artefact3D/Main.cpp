#include "Game.h"

#include <ppltasks.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	HRESULT hr = S_OK;

#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	std::shared_ptr<Game> Instance = std::shared_ptr<Game>(new Game());

	hr = Instance->CreateDesktopWindow();

	if (SUCCEEDED(hr))
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		std::shared_ptr<DeviceManager> deviceManager = std::shared_ptr<DeviceManager>(new DeviceManager());
		std::shared_ptr<ModelLoader> mLoader = std::shared_ptr<ModelLoader>(new ModelLoader(deviceManager));
		std::shared_ptr<World> world = std::shared_ptr<World>(new World());
		std::shared_ptr<RenderingManager> renderer = std::shared_ptr<RenderingManager>(new RenderingManager(deviceManager, mLoader, &world));

		deviceManager->CreateDeviceResources();
		renderer->CreateDeviceDependantResources();

		deviceManager->CreateWindowResources(Instance->GetWindowHandle());
		renderer->CreateWindowSizeDependantResources();

		//deviceManager->GoFullScreen();
		//renderer->CreateDeviceDependantResources();

		hr = Instance->Run(deviceManager, renderer, world);
		CoUninitialize();
	}

	return hr;
}
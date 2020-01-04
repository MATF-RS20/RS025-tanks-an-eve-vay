#include <Windows.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")

#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	HRESULT hr =S_OK;
	if (FAILED(hr))
	{
		ErrorLogger::Log(E_INVALIDARG, "Test Message");
	}

	Engine engine;
	engine.Initialize(hInstance, "Title", "My WIndow Class", 800, 600);

	while (engine.ProcessMessages())
	{
		engine.Update();
		engine.RenderFrame();
	}

	return 0;
}
#include <Windows.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")

#include "Engine.h"
#include "../LoggerTool/Test.h"
#define BREAK

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	Test();
#ifdef BREAK
	return 0;
#endif
	Engine engine;
	engine.Initialize(hInstance, "Title", "My WIndow Class", 1920, 1080);

	while (engine.ProcessMessages())
	{
		engine.Update();
		engine.RenderFrame();
	}

	return 0;
}
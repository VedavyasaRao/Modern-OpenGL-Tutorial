#include "Scene.h"

Scene scene;

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd_line, int show)
{
	DrawTextUtil::Startup();

	scene.Init(RECT{ 100, 100, 780, 500 }, L"Modern OpenGL-Tutorial - Lesson09");
	scene.ShowWindow(show);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	DrawTextUtil::Shutdown();
	return 0;
}
#include "Scene.h"

int ghWnd = 0;
const int WIDTH = 680;
const int HEIGHT = 400;
Scene scene;

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd_line, int show)
{
	DrawTextUtil::Startup();
	scene.Init(RECT{ 100, 100, 100+WIDTH, 100+HEIGHT }, L"Modern OpenGL-Tutorial Lesson07");
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
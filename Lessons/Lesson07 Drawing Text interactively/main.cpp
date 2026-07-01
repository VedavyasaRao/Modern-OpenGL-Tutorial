#include "Scene.h"

int ghWnd = 0;
const int WIDTH = 600;
const int HEIGHT = 600;
Scene scene;

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd_line, int show)
{
	TextImageSketcher::Startup();
	scene.Init(RECT{ 100, 100, 780, 500 }, L"Lesson07: Drawing Text interactively");
	

	
	scene.ShowWindow(show);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
	TextImageSketcher::Shutdown();

	return 0;
}
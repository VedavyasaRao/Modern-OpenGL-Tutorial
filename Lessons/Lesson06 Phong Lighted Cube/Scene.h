#include "Scene\BaseScene.h"
#include "Scene\Camera\ThreeDCamera.h"
#include "Ligting\LightingUtil.h"
#include "Geometry\Cube\PhongLightedTexCube.h"
#include "InputDlg.h"

DWORD WINAPI ThreadFunction(LPVOID lpParam);

class Scene:public BaseScene
{
public:
	//message handler
	BEGIN_MSG_MAP(Scene0)
		MESSAGE_HANDLER(WM_CLOSE, OnCloseWindow)
		COMMAND_ID_HANDLER(IDOK, OnDoRefresh)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
		CHAIN_MSG_MAP(BaseScene)
	END_MSG_MAP()



	//override
	int Init(RECT rect, WCHAR *windowname)
	{
		//create host window and context
		BaseScene::Init(rect, windowname);

		//attach mouse keyboard input handler
		camera = new ThreeDCamera(m_hWnd);
		CreateThread(NULL, 0, ThreadFunction, this, 0, NULL);
		::Sleep(500);

		pdlg->update(&cube.light);

		//Create cube an set texture filename
		cube.Init(true, 0, R"(..\resources\textures\rocks2.bmp)");
		
		return 0;
	}

	//release resources
	void Cleanup()
	{
		cube.Cleanup();
		delete camera;
		
	}
	
	LRESULT OnDoRefresh(WORD wParam, WORD wParam2, HWND lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		pdlg->update(&cube.light);
		Invalidate();
		return 0;
	}

	LRESULT OnClose(WORD wParam, WORD wParam2, HWND lParam, BOOL& bHandled)
	{
		return OnCloseWindow(0, 0, (LPARAM)nullptr, bHandled);
	}


	//draw the scene
	void DrawScene()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//get model view projection matrix. 
		//only model is modified
		//view and projection will be identity matrix
		SceneCamera()->augumentModelMatrix(cube);
		cube.Draw();
		SceneCamera()->MM.Reset();
	}

	//Close the window
	LRESULT OnCloseWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		DestroyWindow();
		PostQuitMessage(0);
		return 0;
	}

	inline ThreeDCamera*  SceneCamera()
	{
		static auto ret = dynamic_cast<ThreeDCamera*>(camera);
		return ret;
	}

	void CreateInputDlg()
	{
		pdlg = new InputDlg();
		pdlg->Create(m_hWnd);
		pdlg->ShowWindow(SW_SHOW);
		Invalidate();
	}


private:
	PhongLightedTexCube cube;
	int IDM_INPUTDLG = 1001;
	InputDlg* pdlg;

};

DWORD WINAPI ThreadFunction(LPVOID lpParam)
{
	Scene* pscene = (Scene*)lpParam;
	pscene->CreateInputDlg();

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return 0;
}
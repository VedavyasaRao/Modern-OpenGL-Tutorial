#include "Scene\BaseScene.h"
#include "Scene\Camera\ThreeDCamera.h"
#include "Geometry\Cube\MultiColoredCube.h"
#include "Scene\Camera\DualProjectionCamera.h"
#include "InputDialog.h"

DWORD WINAPI ThreadFunction(LPVOID lpParam);

class Scene:public BaseScene
{
public:

	BEGIN_MSG_MAP(Scene0)
		MESSAGE_HANDLER(WM_CLOSE, OnCloseWindow)
		COMMAND_ID_HANDLER(IDOK, OnDoRefresh)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
		CHAIN_MSG_MAP(BaseScene)
	END_MSG_MAP()


	int Init(RECT rect, WCHAR *windowname)
	{
		//custom texture mapping
		BaseScene::Init(rect, windowname);
		//attach mouse keyboard input handler
		mskbd = new DualProjectionCamera(m_hWnd);

		SceneCamera()->updateWH();
		SceneCamera()->VM.setViewMatrix({ 0.0,0.0,5.0 }, { 0.0,0.0,0.0 }, { 0.0,1.0,0.0 });
		SceneCamera()->PPM.setProjectionMatrix(1.0,100.0);
		cube.Init();

		CreateThread(NULL, 0, ThreadFunction,  this,  0, NULL);

		return 0;
	}

	void Cleanup()
	{
		cube.Cleanup();
	}
	
	void DrawScene()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SceneCamera()->augumentModelMatrix(cube);
		SceneCamera()->setViewMatrix(cube);
		if (pdlg && pdlg->bortho)
			SceneCamera()->setOrthographicProjectionMatrix(cube);
		else
			SceneCamera()->setPerspectiveProjectionMatrix(cube);
		cube.Draw();
		SceneCamera()->MM.Reset();
	}

	void CreateInputDlg()
	{
		pdlg = new InputDlg();
		pdlg->aspectratio = -SceneCamera()->PPM.AspectRatio;
		pdlg->xminmaxvec = { -pdlg->aspectratio, pdlg->aspectratio };
		pdlg->Create(m_hWnd);
		pdlg->ShowWindow(SW_SHOW);
	}

	LRESULT OnDoRefresh(WORD wParam, WORD wParam2, HWND lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		if (pdlg->blookat)
		{
			SceneCamera()->VM.setViewMatrix(pdlg->positionvec, pdlg->targetvec, pdlg->upvec);
		}

		if (pdlg->bperspective)
		{
			SceneCamera()->PPM.setFOV(pdlg->fovflt);
			SceneCamera()->PPM.setProjectionMatrix(pdlg->nearflt, pdlg->farflt);
		}

		if (pdlg->bortho)
		{
			SceneCamera()->OPM.setProjectionMatrix(pdlg->xminmaxvec, pdlg->yminmaxvec, pdlg->zminmaxvec);
		}

		Invalidate();
		return 0;
	}

	LRESULT OnCloseWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		OnClose(0, 0, 0, bHandled);
		return 0;
	}

	LRESULT OnClose(WORD wParam, WORD wParam2, HWND lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		DestroyWindow();
		PostQuitMessage(0);
		return 0;
	}

	inline DualProjectionCamera*  SceneCamera() { return dynamic_cast<DualProjectionCamera*>(mskbd); }

private:
	MultiColoredCube cube;
	int IDM_INPUTDLG = 1001;
	InputDlg *pdlg;
};
/////////////////////Scene0///////////////////////////////////

DWORD WINAPI ThreadFunction(LPVOID lpParam)
{
	Scene *pscene = (Scene*)lpParam;
	pscene->CreateInputDlg();

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return 0;
}
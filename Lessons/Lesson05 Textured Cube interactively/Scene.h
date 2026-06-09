#include "Canvas\Scene\Base\BaseScene.h"
#include "Canvas\Camera\ThreeDCamera.h"
#include "Geometry\Objects\Cube\TexturedCube.h"
#include "InputDlg.h"

DWORD WINAPI ThreadFunction(LPVOID lpParam);


class Scene:public BaseScene
{
public:
	//message handler
	BEGIN_MSG_MAP(Scene0)
		MESSAGE_HANDLER(WM_CLOSE, OnCloseWindow)
		COMMAND_ID_HANDLER(IDAPPLY, OnDoRefresh)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
		CHAIN_MSG_MAP(BaseScene)
	END_MSG_MAP()

	//override
	int Init(RECT rect, WCHAR *windowname)
	{
		//create host window and context
		BaseScene::Init(rect, windowname);

		CreateThread(NULL, 0, ThreadFunction, this, 0, NULL);

		//attach mouse keyboard input handler
		camera = new ThreeDCamera(m_hWnd);

		::Sleep(500);

		updatecube();

		return 0;
	}

	//release resources
	void Cleanup()
	{
		pcube->Cleanup();
		delete camera;
	}

	void updatecube()
	{

		if (pcube == nullptr)
		{
			BOOL b;
			pdlg->OnBnClickedReset(0, 0, nullptr, b);
		}

		if (pcube != nullptr)
		{
			pcube->Cleanup();
			delete pcube;
			pcube = nullptr;
		}

		if (pcube == nullptr)
		{
			pcube = new TexturedCube();
			pcube->Init(pdlg->getdata());
		}
	}

	LRESULT OnDoRefresh(WORD wParam, WORD wParam2, HWND lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		updatecube();
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
		SceneCamera()->augumentModelMatrix(*pcube);

		pcube->Draw();

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
	InputDlg* pdlg;
	TexturedCube* pcube = nullptr;

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
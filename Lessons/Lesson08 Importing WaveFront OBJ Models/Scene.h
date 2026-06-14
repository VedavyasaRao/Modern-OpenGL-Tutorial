#include "Canvas\Scene\Base\BaseScene.h"
#include "Canvas\Camera\ThreeDCamera.h"
#include "Geometry\Objects\Generic\WaveFrontObj.h"
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

		//attach keyboard/mouse input handler
		camera = new ThreeDCamera(m_hWnd);

		::Sleep(500);

		updateshape();

		return 0;
	}

	//release resources
	void Cleanup()
	{
		if (pshape != nullptr)
			pshape->Cleanup();
		delete camera;
	}
	
	void updateshape()
	{
		if (pshape == nullptr)
		{
			BOOL b;
			pdlg->OnBnClickedReset(0, 0, nullptr, b);

		}

		if (pshape != nullptr)
		{
			pshape->Cleanup();
			delete pshape;
			pshape = nullptr;
		}

		if (pshape == nullptr)
		{
			auto shapeinf = pdlg->getdata();
			pshape = new WaveFrontObj();
			pshape->Init(shapeinf);
		}
	}

	LRESULT OnDoRefresh(WORD wParam, WORD wParam2, HWND lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		updateshape();
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
		if (pshape != nullptr)
		{
			SceneCamera()->augumentModelMatrix(*pshape);
			pshape->Draw();
			SceneCamera()->MM.Reset();
		}
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
	WaveFrontObj *pshape;
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
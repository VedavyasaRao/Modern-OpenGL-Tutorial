#include "Canvas\Scene\Base\BaseScene.h"
#include "Canvas\Camera\ThreeDCamera.h"
#include "Geometry\Utils\Lighting\LightingUtil.h"
#include "Geometry\Objects\Cube\LightedTexCube.h"
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

		CreateThread(NULL, 0, ThreadFunction, this, 0, NULL);

		//attach mouse keyboard input handler
		camera = new ThreeDCamera(m_hWnd);
		::Sleep(500);
		updatecube();

		return 0;
	}

	void updatecube()
	{
		if (pcube != nullptr)
		{
			auto src = pcube->light.lightsrc.src;
			pdlg->update(&pcube->light);
			if (src != pcube->light.lightsrc.src)
			{
				pcube->Cleanup();
				delete pcube;
				pcube = nullptr;
			}
		}

		if (pcube == nullptr)
		{
			pcube = new LightedTexCube();
			pdlg->update(&pcube->light);
			pcube->Init(0, R"(..\resources\textures\rocks.bmp)");
		}
	}

	//release resources
	void Cleanup()
	{
		pcube->Cleanup();
		delete camera;
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

		SceneCamera()->augumentModelMatrix(*pcube);
		wstring wcap = L"Lighting Settings - " + pcube->getangless();
		SetWindowTextW(wcap.c_str());
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
	LightedTexCube *pcube = nullptr;
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
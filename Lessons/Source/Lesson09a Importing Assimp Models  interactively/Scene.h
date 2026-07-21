#include "Canvas\Scene\Base\BaseScene.h"
#include "Canvas\Camera\ThreeDCamera.h"
#include "Geometry\Utils\Generic\GenericParser.h"
#include "Geometry\Objects\Generic\GenericObj.h"
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
		cleanupshapes();
		delete camera;
	}
	
	void updateshape()
	{
		if (shapes.empty())
		{
			paiobjparser =  new AssImpParser;
			BOOL b;
			pdlg->OnBnClickedReset(0, 0, nullptr, b);
		}

		if (!shapes.empty())
		{
			cleanupshapes();
			if (paiobjparser->objfilename  != pdlg->getfilename(pdlg->objfilename))
				paiobjparser->clear();
		}

		if (shapes.empty())
		{
			auto shapeinf = pdlg->getdata();
			if (paiobjparser->objfilename != pdlg->getfilename(pdlg->objfilename))
				paiobjparser->Parse(pdlg->getfilename(pdlg->objfilename));

			for (uint i = 0; i < paiobjparser->meshlst.size(); ++i)
			{
				auto pshape = new GenericObj();
				pshape->Init(i, shapeinf, paiobjparser);
				shapes.push_back(pshape);
			}

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
		ULONG bclr =  pdlg->getblclr();

		glClearColor(GetRValue(bclr)/255.0f, GetGValue(bclr) / 255.0f, GetBValue(bclr) / 255.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST); // Enabling depth testing allows rear faces of 3D objects to be hidden behind front faces.
		glEnable(GL_MULTISAMPLE); // Anti-aliasing
		glEnable(GL_BLEND); // GL_BLEND for OpenGL transparency which is further set within the fragment shader.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (auto pshape : shapes)
		{
			SceneCamera()->augumentModelMatrix(*pshape);
			pshape->Draw();
		}
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
		pdlg = new InputDlg(paiobjparser);
		pdlg->Create(m_hWnd);
		pdlg->ShowWindow(SW_SHOW);
		Invalidate();
	}

	void cleanupshapes()
	{
		for (uint i = 0; i < shapes.size(); ++i)
		{
			shapes[i]->Cleanup();
			delete shapes[i];
		}
		shapes.clear();
	}

private:
	vector<GenericObj*> shapes;
	InputDlg* pdlg;
	AssImpParser *paiobjparser;

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
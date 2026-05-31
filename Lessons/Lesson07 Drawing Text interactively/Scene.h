#include "Canvas\Scene\Base\BaseScene.h"
#include "Canvas\Camera\ThreeDCamera.h"
#include "Geometry\Objects\TextImage\TextImageSketcher.h"
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

	void createnew()
	{
		if (ptextutl)
		{
			ptextutl->Cleanup();
			delete ptextutl;
		}

		ptextutl = new TextImageSketcher();
		ptextutl->Init(GL_TEXTURE0 + 4, pdlg->wd, pdlg->ht);
	}

	int Init(RECT rect, WCHAR *windowname)
	{
		//custom texture mapping
		BaseScene::Init(rect, windowname);
		camera = nullptr;
		CreateThread(NULL, 0, ThreadFunction, this, 0, NULL);
		::Sleep(500);
		createnew();
		return 0;
	}

	void Cleanup()
	{
		ptextutl->Cleanup();
		delete ptextutl;
	}
	
	void DrawScene()
	{
		glClearColor(0, 1, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ptextutl->ClearCanvas(true);
		ptextutl->Drawtext({ pdlg->txtx,pdlg->txty }, pdlg->text,pdlg->hf, &pdlg->fmt, pdlg->pbrush);
		ptextutl->Drawimage({ pdlg->imgx,pdlg->imgy }, pdlg->imagfilename, pdlg->imgclipwd, pdlg->imgclipht);
		ptextutl->DrawCanvas();
	}

	void CreateInputDlg()
	{
		pdlg = new InputDlg();
		pdlg->Create(m_hWnd);
		pdlg->ShowWindow(SW_SHOW);
	}

	LRESULT OnDoRefresh(WORD wParam, WORD wParam2, HWND lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		if ((LPARAM)lParam == 0)
		{
			createnew();
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

	inline ThreeDCamera*  SceneCamera()
	{
		static auto ret = dynamic_cast<ThreeDCamera*>(camera);
		return ret;
	}

private:
	int IDM_INPUTDLG = 1001;
	InputDlg *pdlg;
	TextImageSketcher *ptextutl;


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
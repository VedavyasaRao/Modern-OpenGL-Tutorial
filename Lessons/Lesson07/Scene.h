#include "Scene\BaseScene.h"
#include "Geometry\Text\DrawTextUtil.h"
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
		mskbd = new SimpleCamera(m_hWnd);
		textutl.Init(GL_TEXTURE0+4, 256);
		CreateThread(NULL, 0, ThreadFunction,  this,  0, NULL);
		return 0;
	}

	void Cleanup()
	{
		textutl.Cleanup();
	}
	
	void DrawScene()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mskbd->augumentModelMatrix(textutl);
		textutl.Drawtext({ 1.0,1.0 }, L"Hellow orld!");
		mskbd->MM.Reset();
		

	}

	void CreateInputDlg()
	{
		pdlg = new InputDlg();
		pdlg->Create(m_hWnd);
		pdlg->ShowWindow(SW_SHOW);
		textutl.UpdateFontandColor(pdlg->rgbCurrent, &pdlg->lf);
		Invalidate();
	}

	LRESULT OnDoRefresh(WORD wParam, WORD wParam2, HWND lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		if (pdlg->btranslate)
		{
			textutl.MM.Translateby = glm::vec3(pdlg->tx, pdlg->ty, pdlg->tz);
		}
		textutl.UpdateFontandColor(pdlg->rgbCurrent, &pdlg->lf);
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


private:
	int IDM_INPUTDLG = 1001;
	InputDlg *pdlg;
	DrawTextUtil  textutl;

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
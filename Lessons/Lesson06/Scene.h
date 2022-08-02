#include "Scene\BaseScene.h"
#include "..\Lessons\Lesson04\TexturedCube.h"
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
		std::vector < glm::vec2> texturemap =
		{
			//1 (top)
			{ 0.0f,  0.99f },
			{ 0.33f, 0.99f },
			{ 0.33f, 0.66f },
			{ 0.33f, 0.66f },
			{ 0.0f,  0.66f },
			{ 0.0f,  0.99f },

			//2 (left )
			{ 0.33f, 0.99f },
			{ 0.66f, 0.99f },
			{ 0.66f, 0.66f },
			{ 0.66f, 0.66f },
			{ 0.33f, 0.66f },
			{ 0.33f, 0.99f },

			//3 (bottom)
			{ 0.66f, 0.99f },
			{ 0.99f, 0.99f },
			{ 0.99f, 0.66f },
			{ 0.99f, 0.66f },
			{ 0.66f, 0.66f },
			{ 0.66f, 0.99f },

			//4 (right)
			{ 0.0f,  0.66f },
			{ 0.0f,  0.33f },
			{ 0.33f, 0.33f },
			{ 0.33f, 0.33f },
			{ 0.33f, 0.66f },
			{ 0.0f,  0.66f },

			//5 (back)
			{ 0.33f, 0.66f },
			{ 0.33f, 0.33f },
			{ 0.66f, 0.33f },
			{ 0.66f, 0.33f },
			{ 0.66f, 0.66f },
			{ 0.33f, 0.66f },

			//6 (front)
			{ 0.66f, 0.66f },
			{ 0.99f, 0.66f },
			{ 0.99f, 0.33f },
			{ 0.99f, 0.33f },
			{ 0.66f, 0.33f },
			{ 0.66f, 0.66f },
		};

		BaseScene::Init(rect, windowname);
		//attach mouse keyboard input handler
		mskbd = new SimpleCameraInputHandler(m_hWnd);

		cube.Init(0, R"(..\resources\textures\uvtemplate.tga)");
		dynamic_cast<CubeMesh*>(cube.mesh)->updateTextureMap(texturemap);
		cube.GenerateVertices();

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
		mskbd->fetchCameraData(&cube.camera);
		cube.Draw(false);

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
		if (pdlg->btranslate)
			cube.camera.updateTranslate(glm::vec3(pdlg->tx, pdlg->ty, pdlg->tz));
		if (pdlg->brotate)
		{
			cube.camera.updatePitch(pdlg->rx);
			cube.camera.updateYaw(pdlg->ry);
			cube.camera.updateRoll(pdlg->rz);
		}
		if (pdlg->bscale)
			cube.camera.updateScale(glm::vec3(1.0+pdlg->sx, 1.0+pdlg->sy, 1.0+pdlg->sz));
		mskbd->saveCameraData(&cube.camera);

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
	TexturedCube cube;
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
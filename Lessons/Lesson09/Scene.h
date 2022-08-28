#include "Scene\BaseScene.h"
#include "Scene\Camera\FPSCamera.h"
#include "Geometry\Text\DrawTextUtil.h"
#include "Geometry\Cube\TexturedCube.h"

class Scene:public BaseScene
{
public:
	//message handler
	BEGIN_MSG_MAP(Scene0)
		MESSAGE_HANDLER(WM_CLOSE, OnCloseWindow)
		CHAIN_MSG_MAP(BaseScene)
	END_MSG_MAP()

	//override
	int Init(RECT rect, WCHAR *windowname)
	{
		//create host window and context
		BaseScene::Init(rect, windowname);
		//attach mouse keyboard input handler
		mskbd = new FPSCamera(m_hWnd);
		mskbd->updateWH();
		mskbd->CenterCursor();
		mskbd->PPM.setFOV(60.0f);
		mskbd->PPM.setProjectionMatrix(0.1f, 10.0f);
		cube.Init(0, R"(..\resources\textures\bricks2.jpg)");
		cube.MM.Translateby = glm::vec3(0.0f, 0.0f, -3.0f);
		//generate vertices
		cube.GenerateVertices();
		
		floor.Init(1, R"(..\resources\textures\grid.jpg)");
		floor.MM.Translateby = glm::vec3(0.0f, -1.0f, 0.0f);
		floor.MM.Scaleby = glm::vec3(10.0f, 0.02f, 10.0f);
		floor.GenerateVertices();
		
		LOGFONTW lf;
		ZeroMemory(&lf, sizeof lf);
		lf.lfWidth = 16;
		wcscpy_s(lf.lfFaceName, 32, L"Ariel");
		textutl.Init(GL_TEXTURE0 + 4, 256);
		textutl.MM.Translateby = glm::vec3(-0.2f, 0.3f, 0.0f);
		textutl.UpdateFontandColor(0x000000FF, &lf);

		return 0;
	}

	//release resources
	void Cleanup()
	{
		cube.Cleanup();
		floor.Cleanup();
		textutl.Cleanup();
		delete mskbd;
	}
	
	//draw the scene
	void DrawScene()
	{
		glClearColor(0.0f, 0.0f, 255.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mskbd->augumentModelMatrix(cube);
		((FPSCamera*)mskbd)->updateViewMatrix();

		mskbd->setViewMatrix(cube);
		mskbd->setPerspectiveProjectionMatrix(cube);
		cube.Draw();
		mskbd->MM.Reset();

		mskbd->augumentModelMatrix(textutl);
		textutl.Drawtext(PointF(0.0, 0.0), const_cast<WCHAR*>(L""));

		mskbd->augumentModelMatrix(floor);
		mskbd->setViewMatrix(floor);
		mskbd->setPerspectiveProjectionMatrix(floor);
		floor.Draw();
	}

	//Close the window
	LRESULT OnCloseWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		DestroyWindow();
		PostQuitMessage(0);
		return 0;
	}

private:
	TexturedCube cube;
	TexturedCube floor;
	DrawTextUtil  textutl;
	double previousSeconds = GetTickCount() / 1000.0;
};

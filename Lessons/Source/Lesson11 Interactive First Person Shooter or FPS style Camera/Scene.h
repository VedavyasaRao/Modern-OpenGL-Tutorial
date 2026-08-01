#include "Canvas\Scene\Base\BaseScene.h"
#include "Canvas\Camera\FPSCamera.h"
#include "Geometry\Objects\TextImage\TextImageSketcher.h"
#include "Geometry\Objects\Cube\TexturedCube.h"

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
		camera = new FPSCamera(m_hWnd);

		SceneCamera()->updateWH();
		SceneCamera()->CenterCursor();
		SceneCamera()->PPM.setFOV(60.0f);
		SceneCamera()->PPM.setProjectionMatrix(0.1f, 10.0f);

		cube.Init(TextureInfo(R"(..\resources\textures\bricks.jpg)"));
		cube.MM.Translateby = glm::vec3(0.0f, 0.0f, -2.0f);
		
		//cube.MM.Translateby = glm::vec3(-15.0f, 0.0f, 0.0f);
		//cube.MM.Scaleby = glm::vec3(20.0f, 10.0f, 10.0f);
		
		floor.Init(TextureInfo(R"(..\resources\textures\grid.jpg)"));
		floor.MM.Translateby = glm::vec3(0.0f, -1.0f, 0.0f);
		floor.MM.Scaleby = glm::vec3(10.0f, 0.02f, 10.0f);
		

		LOGFONTW lf;
		ZeroMemory(&lf, sizeof lf);
		lf.lfHeight = -27;
		lf.lfWeight = 400;
		lf.lfClipPrecision = 2;
		lf.lfOutPrecision = 3;
		lf.lfQuality = 1;
		lf.lfPitchAndFamily = 34;
		wcscpy_s(lf.lfFaceName, 32, L"Segoe UI Emoji");
		hfont = CreateFontIndirectW(&lf);
		Color gdipColor;
		gdipColor.SetFromCOLORREF(0x00ff00ff);
		pbrush = new SolidBrush(gdipColor);

		textutl.Init(TextureInfo(), 256,256);
		textutl.MM.Translateby = glm::vec3(-0.2f, 0.3f, 0.0f);
		textutl.ClearCanvas();
		textutl.Drawtext(PointF(0.8f, 0.8f), ss.substr(), hfont, &fmt, pbrush);
		textutl.FlipYAxis();
		textutl.DrawCanvas();

		return 0;
	}

	//release resources
	void Cleanup()
	{
		cube.Cleanup();
		floor.Cleanup();
		delete camera;
		delete pbrush;
		textutl.Cleanup();
		DeleteObject(hfont);
	}
	
	//draw the scene
	void DrawScene()
	{
		
		glClearColor(0.0f, 0.0f, 255.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SceneCamera()->augumentModelMatrix(cube);
		SceneCamera()->updateViewMatrix();

		SceneCamera()->setViewMatrix(cube);
		SceneCamera()->setPerspectiveProjectionMatrix(cube);
		cube.Draw();
		SceneCamera()->MM.Reset();

		SceneCamera()->augumentModelMatrix(textutl);
		

		SceneCamera()->augumentModelMatrix(floor);
		SceneCamera()->setViewMatrix(floor);
		SceneCamera()->setPerspectiveProjectionMatrix(floor);
		floor.Draw();

		textutl.Draw();

	}

	//Close the window
	LRESULT OnCloseWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		DestroyWindow();
		PostQuitMessage(0);
		return 0;
	}

	inline FPSCamera*  SceneCamera()
	{
		static auto ret = dynamic_cast<FPSCamera*>(camera);
		return ret;
	}

private:
	TexturedCube cube;
	TexturedCube floor;
	TextImageSketcher  textutl;
	HFONT  hfont;
	StringFormat fmt;
	SolidBrush* pbrush;
	double previousSeconds = GetTickCount() / 1000.0;
	wstring ss = L"Khri$ha";
};

#include "Canvas\Scene\Base\BaseScene.h"
#include "Canvas\Camera\OrbitCamera.h"
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
		camera = new OrbitCamera(m_hWnd);

		SceneCamera()->updateWH();
		SceneCamera()->CenterCursor();

		SceneCamera()->PPM.setFOV(45.0f);
		SceneCamera()->PPM.setProjectionMatrix(0.1f, 100.0f);
		cube.Init(TextureInfo(R"(..\resources\textures\rocks.bmp)"));
		cube.MM.Translateby = glm::vec3(0.0f, 0.0f, -3.0f);
		
		floor.Init(TextureInfo(R"(..\resources\textures\grid.jpg)"));
		floor.MM.Translateby = glm::vec3(0.0f, -1.0f, 0.0f);
		floor.MM.Scaleby = glm::vec3(10.0f, 0.02f, 10.0f);
		
		return 0;
	}

	//release resources
	void Cleanup()
	{
		cube.Cleanup();
		floor.Cleanup();
		delete camera;
	}
	
	//draw the scene
	void DrawScene()
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SceneCamera()->augumentModelMatrix(cube);
		SceneCamera()->updateViewMatrix();

		SceneCamera()->setViewMatrix(cube);
		SceneCamera()->setPerspectiveProjectionMatrix(cube);
		cube.Draw();
		SceneCamera()->MM.Reset();


		SceneCamera()->augumentModelMatrix(floor);
		SceneCamera()->setViewMatrix(floor);
		SceneCamera()->setPerspectiveProjectionMatrix(floor);
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

	inline OrbitCamera*  SceneCamera()
	{
		static auto ret = dynamic_cast<OrbitCamera*>(camera);
		return ret;
	}

private:
	TexturedCube cube;
	TexturedCube floor;

	double previousSeconds = GetTickCount() / 1000.0;
};

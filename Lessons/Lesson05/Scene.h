#include "Scene\BaseScene.h"
#include "Scene\Camera\ThreeDCamera.h"
#include "Geometry\Cube\MultiColoredCube.h"

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
		camera = new ThreeDCamera(m_hWnd);

		//Create multicolor cube
		cube.Init();
		
		return 0;
	}

	//release resources
	void Cleanup()
	{
		cube.Cleanup();
		delete camera;
		
	}
	
	//draw the scene
	void DrawScene()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//get model view projection matrix. 
		//only model is modified
		//view and projection will be identity matrix
		SceneCamera()->augumentModelMatrix(cube);
		cube.Draw();
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

private:
	MultiColoredCube cube;

};

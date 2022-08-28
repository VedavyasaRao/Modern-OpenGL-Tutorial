#include "Scene\BaseScene.h"
#include "Geometry\Cube\SingleColoredCube.h"

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
		//attach keyboard/mouse input handler
		mskbd = new SimpleCamera(m_hWnd);

		//Create cube an set color
		cube.Init(glm::vec3(1.0f,0.0f,1.0f));
		return 0;
	}

	//release resources
	void Cleanup()
	{
		cube.Cleanup();
		delete mskbd;
	}
	
	//draw the scene
	void DrawScene()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mskbd->augumentModelMatrix(cube);
		cube.Draw();
		mskbd->MM.Reset();
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
	SingleColoredCube cube;
};

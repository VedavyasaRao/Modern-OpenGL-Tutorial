#include "Scene\BaseScene.h"
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
		mskbd = new SimpleCamera(m_hWnd);
		//Create cube an set texture filename
		cube.Init(0, R"(..\resources\textures\bricks2.jpg)");
		//generate vertices
		cube.GenerateVertices();
		
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
		//get model view projection matrix. 
		//only model is modified
		//view and projection will be identity matrix
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
	TexturedCube cube;
};

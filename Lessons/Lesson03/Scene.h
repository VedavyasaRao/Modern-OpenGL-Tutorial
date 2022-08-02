#include "Scene\BaseScene.h"
#include "IndexedCube.h"

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
		mskbd = new SimpleCameraInputHandler(m_hWnd);

		//Create cube an set color
		cube.Init();
		
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
		mskbd->fetchCameraData(&cube.camera);
		cube.Draw();

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
	IndexedCube cube;
};

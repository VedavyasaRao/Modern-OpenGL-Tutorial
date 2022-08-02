#include "Scene\BaseScene.h"
#include "..\Lessons\Lesson05\MultiColoredCube.h"
#include "..\..\VOGLLib\Scene\Camera\PerspectiveProjectionCameraInputHandler.h"
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
		mskbd = new PerspectiveProjectionCameraInputHandler(m_hWnd);

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
		mskbd->UpdateViewMatrix({0.0,-5.0,0.0}, { 0.0,0.0,0.0}, { 1.0,1.0,0.0});
		mskbd->UpdateProjectionMatrix(0.5,100.0);
		mskbd->fetchCameraData(&cube.camera);
		cube.Draw(false);
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
	MultiColoredCube cube;
};

#include "Scene\BaseScene.h"

class Scene:public BaseScene
{
	BEGIN_MSG_MAP(Scene)
		MESSAGE_HANDLER(WM_CLOSE, OnCloseWindow)
		CHAIN_MSG_MAP(BaseScene)
	END_MSG_MAP()

	//shutdown the application when the wondow is closed
	LRESULT OnCloseWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		DestroyWindow();
		PostQuitMessage(0);
		return 0;
	}
	
	//override to create application window and opengl context
	int Init(RECT rect, WCHAR *windowname)
	{
		BaseScene::Init(rect, windowname);
		return 0;
	}

	//no cleanup
	void Cleanup()
	{
	}
	
	//draw
	void DrawScene()
	{
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT_AND_BACK);
	}

};

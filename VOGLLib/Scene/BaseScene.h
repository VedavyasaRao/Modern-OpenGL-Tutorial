#pragma once
#include "OGLAppWindow.h"
#include "..\Camera\BaseCameraInputHandler.h"

class BaseScene:public COGLAppWindow
{
	//message handlers for window events
	BEGIN_MSG_MAP(BaseScene)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnMouseBtnDown)
		MESSAGE_HANDLER(WM_RBUTTONDOWN, OnMouseBtnDown)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	//when WM_PAINT is recived
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		PAINTSTRUCT rect;
		HDC hdc = BeginPaint(&rect);
		
		//overridden by deriving class
		DrawScene();

		//double buffering enables painting in one buffer while displaying from the buffer
		SwapBuffers(hdc);

		EndPaint(&rect);
		bHandled = TRUE;
		return 0;
	}

	//Called when the window is resized
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int w = LOWORD(lParam);
		int h = HIWORD(lParam);

		glViewport(0, 0, (GLsizei)w, (GLsizei)h);
		HandleResize(w, h);
		PaintScene();
		bHandled = TRUE;
		return 0;
	}

	//when a key is down. 
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//calls mousekeyboardinput handler if assigned
		HandleKeyboardinput(wParam);
		bHandled = TRUE;
		return 0;
	}

	//when the left mouse button is pressed. 
	LRESULT OnMouseBtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		//calls mousekeyboardinput handler if assigned
		HandleMouseBtnClick(wParam, x, y);
		bHandled = TRUE;
		return 0;
	}

	//when the mouse is moved.
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		//calls mousekeyboardinput handler if assigned
		HandleMouseMove(wParam, x, y);
		bHandled = TRUE;
		return 0;
	}

	//when mouse wheel is scrolled.
	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int fwKeys = GET_KEYSTATE_WPARAM(wParam);
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		//calls mousekeyboardinput handler if assigned
		HandleMouseWheel(zDelta);
		bHandled = TRUE;
		return 0;
	}
	
protected:
	//called when the application window is closed
	BOOL DestroyWindow()
	{
		//overridden by deriving class to release resources 
		Cleanup();

		//discard the context and close the window
		COGLAppWindow::DestroyWindow();
		return 0;
	}

	//force repaint 
	void PaintScene()
	{
		Invalidate(FALSE);
	}

	//called when the application window is setup
	virtual int Init(RECT rect, WCHAR *windowname)
	{
		//Load openGL extensions
		if (init_opengl_extensions() != 0)
			return 1;

		//Create the application window
		if (!Create(NULL, rect, windowname, WS_OVERLAPPEDWINDOW | WS_VISIBLE))
			return 2;

		//Create OpenGL context
		return init_opengl();
	}

	//must be overidden by the application to render scene
	virtual void DrawScene() = 0;

	//must be overidden by the application to cleanup 
	virtual void Cleanup() = 0;

	//window resize handler
	virtual void HandleResize(int w, int h)
	{
		if (mskbd == nullptr)
			return;
		mskbd->updateWH();
	}

	//keyboard input handler. calls mousekeyboardinput handler if assigned
	virtual void HandleKeyboardinput(int key)
	{
		if (key == 27)
		{
			SendMessage(WM_CLOSE);
			return;
		}
		
		if (mskbd == nullptr)
			return;

		if (mskbd->OnKey(key))
			PaintScene();
	}

	//mouse left button click handler
	virtual void  HandleMouseBtnClick(int button, int x, int y)
	{
		if (mskbd == nullptr)
			return;
		mskbd->OnMouseBtnDown(button, x, y);
	}

	//mouse move click handler
	virtual void HandleMouseMove(int button, int x, int y)
	{
		if (mskbd == nullptr)
			return;
		if (mskbd->OnMouseMove(button, x, y))
			PaintScene();
	}

	//mouse wheel handler
	virtual void HandleMouseWheel(int dir)
	{
		if (mskbd == nullptr)
			return;
		mskbd->updateFOV(dir);
		PaintScene();
	}

protected:
	//mouse/keyboard input handler
	BaseCameraInputHandler* mskbd;


};


#pragma once
#include <iomanip>

#include "CameraData.h"
#include "..\Geometry\BaseGeometry.h"

class  BaseCamera
{
public:
	BaseCamera(HWND hwnd)
	{
		this->hwnd = hwnd;
	}

	virtual bool OnKey(int key)
	{
		return false;
	}
	

	virtual bool OnMouseMove(int button, int x, int y)
	{
		if (mouseX == 0 || mouseY == 0)
		{
			mouseX = x;
			mouseY = y;
		}

		deltaX = (mouseX - x);
		deltaY = (mouseY - y);

		mouseX = x;
		mouseY = y;

		OnMouseBtnDown(button, x, y);
		return false;
	}

	virtual void OnMouseBtnDown(int button, int x, int y)
	{
		mouseLeftDown = false;
		mouseRightDown = false;
		mouseMiddleDown = false;
		if (button == MK_LBUTTON)
			mouseLeftDown = TRUE;
		else if (button == MK_RBUTTON)
			mouseRightDown = TRUE;
		else if (button == MK_MBUTTON)
			mouseMiddleDown = TRUE;
	}

	virtual bool OnMouseWheel(int dir) 
	{ 
		return false; 
	}

	virtual void updateWH()
	{
		GetClientRect(hwnd, &clientrect);
		GetWindowRect(hwnd, &windowrect);
		mouseX = 0;
		mouseY = 0;
		deltaX = 0;
		deltaY = 0;
	}

	void CenterCursor()
	{
		auto wc = windowrect.left + (windowrect.right - windowrect.left) / 2;
		auto hc = windowrect.top + (windowrect.bottom - windowrect.top) / 2;
		SetCursorPos(wc, hc);
		ClipCursor(&windowrect);
	}
	
protected:
	HWND hwnd;
	RECT clientrect;
	RECT windowrect;
	bool mouseLeftDown = false;
	bool mouseRightDown = false;
	bool mouseMiddleDown = false;
	int mouseX = 0;
	int mouseY = 0;
	int deltaX = 0;
	int deltaY = 0;
};

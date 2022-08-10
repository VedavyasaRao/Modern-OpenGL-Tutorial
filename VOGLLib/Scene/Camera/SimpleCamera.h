#pragma once
#include "..\CameraData.h"
#include "..\BaseCamera.h"

class SimpleCamera:public BaseCamera
{
public:

	SimpleCamera(HWND hwnd) :BaseCamera(hwnd) {}

	bool OnKey(int key)
	{
		if (BaseCamera::OnKey(key))
			return true;

		switch (key)
		{
			case 'x':
			case 'X':
				MM.Pitch = 10;
				break;

			case 'y':
			case 'Y':
				MM.Yaw = 10;
				break;

			case 'z':
			case 'Z':
				MM.Roll = 10;
				break;

			default:
				return false;
		}
		return true;
	}

	bool OnMouseMove(int button, int x, int y)
	{
		if (BaseCamera::OnMouseMove(button, x, y))
			return true;

		if (mouseLeftDown)
		{
			MM.Pitch = deltaY;
			MM.Yaw = deltaX;
			return true;
		}
		return false;
	}
};

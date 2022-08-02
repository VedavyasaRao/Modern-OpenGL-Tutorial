#pragma once
#include "..\CameraData.h"
#include "..\BaseCameraInputHandler.h"

class SimpleCameraInputHandler:public BaseCameraInputHandler
{
public:

	SimpleCameraInputHandler(HWND hwnd) :BaseCameraInputHandler(hwnd) {}

	bool OnKey(int key)
	{
		if (BaseCameraInputHandler::OnKey(key))
			return true;

		switch (key)
		{
			case 'x':
			case 'X':
				data.augumentpitch(10);
				break;

			case 'y':
			case 'Y':
				data.augumentyaw(10);
				break;

			case 'z':
			case 'Z':
				data.augumentroll(10);
				break;

			default:
				return false;
		}
		return true;
	}

	bool OnMouseMove(int button, int x, int y)
	{
		if (BaseCameraInputHandler::OnMouseMove(button, x, y))
			return true;

		if (mouseLeftDown)
		{
			data.augumentpitch((deltaY));
			data.augumentyaw((deltaX));
			return true;
		}
		return false;
	}
};

#pragma once
#include "BaseCameraInputHandler.h"

/////////////////////PerspectiveProjectionCameraInputHandler///////////////////////////////////
class PerspectiveProjectionCameraInputHandler :public BaseCameraInputHandler
{
public:
	PerspectiveProjectionCameraInputHandler(HWND hwnd) :BaseCameraInputHandler(hwnd) {}

	bool OnKey(int key)
	{
		if (BaseCameraInputHandler::OnKey(key))
			return true;

		switch (key)
		{
			case VK_PRIOR:
				updatefov(1);
				break;

			case VK_NEXT:
				updatefov(-1);
				break;

			default:
				return false;
		}
		return true;
	}

	virtual void UpdateProjectionMatrix()
	{
		data.UpdateProjectionMatrix(perspective(radians((float)fov), aspectratio, 0.1f, 100.0f));
	}
};
/////////////////////PerspectiveProjectionCameraInputHandler///////////////////////////////////


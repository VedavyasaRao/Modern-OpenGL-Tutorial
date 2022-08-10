#pragma once
#include "SimpleCamera.h"

/////////////////////PerspectiveProjectionCameraInputHandler///////////////////////////////////
class PerspectiveProjectionCamera :public SimpleCamera
{
public:
	PerspectiveProjectionCamera(HWND hwnd) :SimpleCamera(hwnd) {}

	bool OnKey(int key)
	{
		if (SimpleCamera::OnKey(key))
			return true;

		switch (key)
		{
			case 'p':
			case 'P':
				OutputDebugStringA(Unproject(PPM.getProjectionMatrix()).c_str());
				break;

			case VK_PRIOR:
				PPM.setFOV(1);
				break;

			case VK_NEXT:
				PPM.setFOV(-1);
				break;

			default:
				return false;
		}
		return true;
	}

	bool OnMouseWheel(int dir)
	{
		PPM.setFOV(dir);
		return true;
	}
	
};
/////////////////////PerspectiveProjectionCameraInputHandler///////////////////////////////////


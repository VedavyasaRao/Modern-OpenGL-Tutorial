#pragma once
#include "SimpleCameraInputHandler.h"

/////////////////////PerspectiveProjectionCameraInputHandler///////////////////////////////////
class PerspectiveProjectionCameraInputHandler :public SimpleCameraInputHandler
{
public:
	PerspectiveProjectionCameraInputHandler(HWND hwnd) :SimpleCameraInputHandler(hwnd) {}

	bool OnKey(int key)
	{
		if (SimpleCameraInputHandler::OnKey(key))
			return true;

		switch (key)
		{
			case VK_PRIOR:
				updateFOV(1);
				break;

			case VK_NEXT:
				updateFOV(-1);
				break;

			default:
				return false;
		}
		return true;
	}

	virtual bool OnMouseWheel(int dir)
	{
		updateFOV(dir);
		return true;
	}

	void UpdateViewMatrix(const vec3& postion, const vec3& target, const vec3& up)
	{
		data.UpdateViewMatrix(lookAt(postion, target, up));
	}

	void UpdateProjectionMatrix(float nearplane, float farplane)
	{
		this->nearplane = nearplane;
		this->farplane = farplane;
		data.UpdateProjectionMatrix(perspective(radians((float)fov), aspectratio, nearplane, farplane));
	}

	void UpdateProjectionMatrix(vec2 xminmax, vec2 yminmax, vec2 zminmax)
	{
		//data.UpdateProjectionMatrix(glm::ortho(-aspectratio, aspectratio, -1.0f, 1.0f, 1.0f, 100.0f));
		data.UpdateProjectionMatrix(glm::ortho(xminmax.r, xminmax.g, yminmax.r, yminmax.g, zminmax.r, zminmax.g));
	}

	void updateFOV()
	{
		if (fov < 10)
			fov = 10;
		if (fov > 135)
			fov = 135;
		UpdateProjectionMatrix(nearplane, farplane);
	}

	void updateFOV(int dir)
	{
		fov += 2 * (std::signbit(dir*1.0) ? -1 : 1);
		updateFOV();
	}

	void updateFOV(float fovin)
	{
		fov = (short)fovin;
		updateFOV();
	}

protected:
	short fov = 45;
	float nearplane = 1.0;
	float farplane = 100.0;

};
/////////////////////PerspectiveProjectionCameraInputHandler///////////////////////////////////


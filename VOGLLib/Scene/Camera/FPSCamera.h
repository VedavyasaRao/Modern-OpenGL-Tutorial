#pragma once
#include "PerspectiveProjectionCamera.h"

/////////////////////FPSCamera///////////////////////////////////
class FPSCamera :public PerspectiveProjectionCamera
{
public:
	FPSCamera(HWND hwnd) :PerspectiveProjectionCamera(hwnd) {}

	bool OnKey(int key)
	{
		if (PerspectiveProjectionCamera::OnKey(key))
			return true;

		switch (key)
		{
		case 'W':
		case 'w':
			camera_pos += camera_front * velocity;
			break;

		case 'S':
		case 's':
			camera_pos -= camera_front * velocity;
			break;

		case 'A':
		case 'a':
			camera_pos += cross(camera_front, camera_up) * velocity;
			break;

		case 'D':
		case 'd':
			camera_pos -= cross(camera_front, camera_up) * velocity;
			break;

		case 'R':
		case 'r':
			camera_pos -= camera_up * velocity;
			break;

		case 'F':
		case 'f':
			camera_pos += camera_up * velocity;
			break;

		default:
			return false;
		}
		return true;
	}

	bool OnMouseMove(int button, int x, int y)
	{
		if (x <= 10 || x >= clientrect.right - 10 || y <= 10 || y >= clientrect.bottom - 10)
		{
			CenterCursor();
			return false;
		}

		if (PerspectiveProjectionCamera::OnMouseMove(button, x, y))
			return true;

		if (!mouseLeftDown && !mouseRightDown)
		{
			camera_front = mat3(rotate((float)-deltaX * mouse_sensitivity, camera_up)) * mat3(rotate((float)-deltaY*mouse_sensitivity, cross(camera_front, camera_up))) * camera_front;
			return true;
		}
		return false;
	}

	void updateViewMatrix()
	{
		VM.setViewMatrix(camera_pos, camera_pos + camera_front, camera_up);
	}

	void setViewMatrix(const vec3& camera_pos, const vec3& camera_front, const vec3&  camera_up)
	{
		this->camera_pos = camera_pos;
		this->camera_front = camera_front;
		this->camera_up = camera_up;
		updateViewMatrix();
	}

	void setSenseivity(const float& velocity, const float& mouse_sensitivity)
	{
		this->velocity = velocity;
		this->mouse_sensitivity = mouse_sensitivity;
	}

protected:
	vec3 camera_pos = { 0.0, 0.0, 0.0 };
	vec3 camera_front = { 0.0, 0.0, -1.0 };
	vec3 camera_up = { 0.0, 1.0, 0.0 };

	// camera options
	float velocity = 0.05f;
	float mouse_sensitivity = 0.01f;
};
/////////////////////FPSCamera///////////////////////////////////


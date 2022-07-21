#pragma once
#include "CameraData.h"

class BaseCameraInputHandler
{
public:
	void *operator new(size_t size)
	{
		return _mm_malloc(size, 16);
	}

	void operator delete(void *ptr)
	{
		_mm_free(ptr);
	}

	BaseCameraInputHandler(HWND hwnd)
	{
		this->hwnd = hwnd;
	}

	virtual bool OnKey(int key)
	{
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

		case 'p':
		case 'P':
			//data.V = V;
			//data.P = P;
			//Unproject();
			break;

		default:
			return false;
		}
		return true;
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
		if (mouseLeftDown)
		{
			data.augumentpitch((deltaY));
			data.augumentyaw((deltaX));
		}
		return (mouseLeftDown)?true:false;
	}

	virtual void OnMouseBtnDown(int button, int x, int y)
	{
		mouseLeftDown = false;
		mouseRightDown = false;
		if (button == MK_LBUTTON)
			mouseLeftDown = TRUE;
		else if (button == MK_RBUTTON)
			mouseRightDown = TRUE;
	}

	virtual wstring GetDisplayString()
	{
		WCHAR buf[300];
		swprintf_s(buf, 300, L"Rotation:%d %d %d", data.pitch, data.yaw, data.roll);
		return buf;
	}

	virtual void UpdateViewMatrix()	{}

	virtual void UpdateProjectionMatrix()	{}

	void updateWH()
	{
		GetClientRect(hwnd, &clientrect);
		GetWindowRect(hwnd, &windowrect);
		aspectratio = (float)clientrect.right / clientrect.bottom;
		mouseX = 0;
		mouseY = 0;
		deltaX = 0;
		deltaY = 0;
	}

	void updateFOV(int dir)
	{
		fov += 2 * (std::signbit(dir*1.0)?-1:1);
		if (fov < 10)
			fov = 10;
		if (fov > 135)
			fov = 135;
	}

	void CenterCursor()
	{
		auto wc = windowrect.left + (windowrect.right - windowrect.left) / 2;
		auto hc = windowrect.top + (windowrect.bottom - windowrect.top) / 2;
		SetCursorPos(wc, hc);
		ClipCursor(&windowrect);

	}

	virtual void fetchCameraData(CameraData *target)
	{
		*target = data;
	}

	virtual void saveCameraData(CameraData *target)
	{
		data = *target;
	}

	//void Unproject()
	//{
	//	auto w = windowrect.right - windowrect.left;
	//	auto h = windowrect.bottom - windowrect.top;

	//	glm::vec4 viewport = glm::vec4(0, 0, w, h);

	//	GLfloat depth;

	//	glReadPixels(mouseX, h - mouseY - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	//	if (depth >= 1.0f)
	//	{
	//		glm::vec3 world_origin{ 0.0f, 0.0f, 0.0f };
	//		glm::vec3 origin_ndc = glm::project(world_origin, V, P, viewport);
	//		depth = origin_ndc[2];
	//	}

	//	glm::vec3 wincoord = glm::vec3(mouseX, h - mouseY - 1, depth);
	//	glm::vec3 objcoord = glm::unProject(wincoord, V, P, viewport);
	//	vec3 projected = glm::project(objcoord, V, P, viewport);

	//	cout << mouseX << " " << mouseY << endl;
	//	cout << objcoord.x << " " << objcoord.y << " " << objcoord.z << endl;
	//	cout << projected.x << " " << h-projected.y << " " << projected.z << endl;
	//}



protected:
	CameraData data;
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
	short fov = 45;
	float aspectratio = 1.7f;
};

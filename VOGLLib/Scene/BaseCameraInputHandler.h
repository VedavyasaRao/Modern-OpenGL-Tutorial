#pragma once
#include <iomanip>
#include "CameraData.h"

class  BaseCameraInputHandler
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
			case 'p':
			case 'P':
				OutputDebugStringA(Unproject().c_str());
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
		return false;
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

	virtual bool OnMouseWheel(int dir) 
	{ 
		return false; 
	}

	virtual wstring GetDisplayString()
	{
		WCHAR buf[300];
		swprintf_s(buf, 300, L"Rotation:%d %d %d", data.pitch, data.yaw, data.roll);
		return buf;
	}

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

	virtual void fetchCameraData(CameraData *target)
	{
		*target = data;
	}

	virtual void saveCameraData(CameraData *target)
	{
		data = *target;
	}

	void CenterCursor()
	{
		auto wc = windowrect.left + (windowrect.right - windowrect.left) / 2;
		auto hc = windowrect.top + (windowrect.bottom - windowrect.top) / 2;
		SetCursorPos(wc, hc);
		ClipCursor(&windowrect);
	}


	string Unproject()
	{
		auto w = clientrect.right - clientrect.left;
		auto h = clientrect.bottom - clientrect.top;

		glm::vec4 vp(0, 0, w, h);

		GLfloat depth;

		glReadPixels(mouseX, h - mouseY - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		if (depth >= 1.0f)
		{
			glm::vec3 world_origin{ 0.0f, 0.0f, 0.0f };
			glm::vec3 origin_ndc = glm::project(world_origin, data.V * data.M , data.P, vp);
			depth = origin_ndc[2];
		}

		glm::vec3 wincoord = glm::vec3(mouseX, h - mouseY - 1, depth);
		glm::vec3 objcoord = glm::unProject(wincoord, data.V * data.M, data.P, vp);
		vec3 projected = glm::project(objcoord, data.V * data.M, data.P, vp);

		std::stringstream ss;

		ss << mouseX << " " << mouseY << " " << std::setprecision(2) << depth << endl;
		ss << std::setprecision(2) << objcoord.x << " " << objcoord.y << " " << objcoord.z << endl;
		//ss << projected.x << " " << h-projected.y << " " << projected.z << endl;
		return ss.str();
	}

	float GetAspectRatio()
	{
		return aspectratio;
	}


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
	float aspectratio = 1.7f;
};

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
		MM.Reset();
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
		if (button == MK_LBUTTON)
			mouseLeftDown = TRUE;
		else if (button == MK_RBUTTON)
			mouseRightDown = TRUE;
	}

	virtual bool OnMouseWheel(int dir) 
	{ 
		return false; 
	}

	void updateWH()
	{
		GetClientRect(hwnd, &clientrect);
		GetWindowRect(hwnd, &windowrect);
		PPM.setAspectRatio((float)clientrect.right / clientrect.bottom);
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

	void augumentModelMatrix(BaseGeometry& geo)
	{
		geo.MM.augument(MM);
	}

	void setViewMatrix(BaseGeometry& geo)
	{
		geo.ViewMatrix = VM.getViewMatrix();
	}

	void setPerspectiveProjectionMatrix(BaseGeometry& geo)
	{
		geo.ProjectionMatrix = PPM.getProjectionMatrix();
	}

	void setOrthographicProjectionMatrix(BaseGeometry& geo)
	{
		geo.ProjectionMatrix = OPM.getProjectionMatrix();
	}

protected:
	string Unproject(const mat4& P)
	{
		auto w = clientrect.right - clientrect.left;
		auto h = clientrect.bottom - clientrect.top;

		glm::vec4 vp(0, 0, w, h);

		GLfloat depth;

		glReadPixels(mouseX, h - mouseY - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		auto M = mat4(1);// MM.getModelMatrix();
		auto V = VM.getViewMatrix();

		if (depth >= 1.0f)
		{
			glm::vec3 world_origin{ 0.0f, 0.0f, 0.0f };
			glm::vec3 origin_ndc = glm::project(world_origin, V * M, P, vp);
			depth = origin_ndc[2];
		}

		glm::vec3 wincoord = glm::vec3(mouseX, h - mouseY - 1, depth);
		glm::vec3 objcoord = glm::unProject(wincoord, V * M, P, vp);
		vec3 projected = glm::project(objcoord, V * M, P, vp);

		std::stringstream ss;

		ss << mouseX << " " << mouseY << " " << std::setprecision(2) << depth << endl;
		ss << std::setprecision(2) << objcoord.x << " " << objcoord.y << " " << objcoord.z << endl;
		//ss << projected.x << " " << h-projected.y << " " << projected.z << endl;
		return ss.str();
	}

public:
	ModelMatrixData MM;
	ViewMatrixData  VM;
	PerspectiveProjectionMatrixData PPM;
	OrthographicProjectionMatrixData OPM;


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

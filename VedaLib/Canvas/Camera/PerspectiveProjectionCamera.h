#pragma once
#include "ThreeDCamera.h"

/////////////////////PerspectiveProjectionCameraInputHandler///////////////////////////////////
class PerspectiveProjectionCamera :public ThreeDCamera
{
public:
	PerspectiveProjectionCamera(HWND hwnd) :ThreeDCamera(hwnd) {}

	bool OnKey(int key)
	{
		if (ThreeDCamera::OnKey(key))
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

	virtual void updateWH()
	{
		ThreeDCamera::updateWH();
		PPM.setAspectRatio((float)clientrect.right / clientrect.bottom);
	}

	void setViewMatrix(BaseGeometry& geo)
	{
		geo.ViewMatrix = VM.getViewMatrix();
	}

	void setPerspectiveProjectionMatrix(BaseGeometry& geo)
	{
		geo.ProjectionMatrix = PPM.getProjectionMatrix();
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
	ViewMatrixData  VM;
	PerspectiveProjectionMatrixData PPM;
};
/////////////////////PerspectiveProjectionCameraInputHandler///////////////////////////////////


#pragma once
#include "ThreeDCamera.h"

/////////////////////PerspectiveProjectionCameraInputHandler///////////////////////////////////
class DualProjectionCamera :public ThreeDCamera
{
public:
	DualProjectionCamera(HWND hwnd) :ThreeDCamera(hwnd) {}

	bool OnKey(int key)
	{
		if (ThreeDCamera::OnKey(key))
			return true;

		switch (key)
		{
			case 'p':
			case 'P':
			{
				auto s = Unproject(PPM.getProjectionMatrix());
				OutputDebugString(s.data());
				CopyToClipboard(s.data());
			}
				break;
			
			//page up
			case VK_PRIOR:
				PPM.setFOV(1);
				break;

			//page down
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

	void setOrthographicProjectionMatrix(BaseGeometry& geo)
	{
		geo.ProjectionMatrix = OPM.getProjectionMatrix();
	}

protected:
	wstring Unproject(const mat4& P)
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

		std::wstringstream ss;

		ss << "mouseX:" << mouseX << L" mouseY:" << mouseY << L" depth:" << std::setprecision(2) << depth << endl;
		ss << std::setprecision(2) << "X:" << objcoord.x << L" Y:" << objcoord.y << L" Z:" << objcoord.z << endl;
		//ss << projected.x << " " << h-projected.y << " " << projected.z << endl;
		return ss.str();
	}

	bool CopyToClipboard(const wchar_t* text) 
	{
		// 1. Open the clipboard associated with the current task
		if (!OpenClipboard(NULL)) {
			return false;
		}

		// 2. Clear the old clipboard contents to assume ownership
		if (!EmptyClipboard()) {
			CloseClipboard();
			return false;
		}

		// Calculate buffer size including the null terminator
		size_t size = (wcslen(text) + 1) * sizeof(wchar_t);

		// 3. Allocate moveable global memory (required for standard clipboard data)
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
		if (hMem == NULL) {
			CloseClipboard();
			return false;
		}

		// 4. Lock the memory block to get a raw pointer and copy the string
		wchar_t* pMem = static_cast<wchar_t*>(GlobalLock(hMem));
		if (pMem != NULL) {
			wcscpy_s(pMem, wcslen(text) + 1, text);
		}
		GlobalUnlock(hMem);

		// 5. Place the data onto the clipboard using Unicode text format
		if (SetClipboardData(CF_UNICODETEXT, hMem) == NULL) {
			GlobalFree(hMem); // Free memory only if the API call fails
			CloseClipboard();
			return false;
		}

		// 6. Close the clipboard so other programs can access it
		CloseClipboard();
		return true;
	}


public:
	ViewMatrixData  VM;
	PerspectiveProjectionMatrixData PPM;
	OrthographicProjectionMatrixData OPM;

};
/////////////////////PerspectiveProjectionCameraInputHandler///////////////////////////////////


#pragma once
#include "CompileLink.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream>

#include <windows.h>
#include <gdiplus.h>
#include <gdiplusheaders.h>
#include <atlbase.h>
#include <atlwin.h>

using namespace Gdiplus;


/******************************
//https://galogen.gpfault.net/galogen-web.html
#include "GL/gl.c"

//https://github.com/KhronosGroup/OpenGL-Registry/blob/main/api/GL/wglext.h
#include "GL/wglext.h"
*******************************/


//https://gen.glad.sh/
//see gen.glad.sh.pdf
#define GLAD_GL_IMPLEMENTATION
#include "GLad/gl.h"

#define GLAD_WGL_IMPLEMENTATION
#include "GLad/wgl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



//This class defines the window that hosts the Modern OpenGL rendering context
class COGLAppWindow : public CWindowImpl<COGLAppWindow>
{
public:
	//define window class of owner draw style
	DECLARE_WND_CLASS_EX(_T("OGLAppWindow"), CS_OWNDC, BLACK_BRUSH)

	//no message handlers
	DECLARE_EMPTY_MSG_MAP()

	//In this step, a dummy window is created and 1.1 compatible rendering context is created
	//LoadWGLExtensions() is called to load wgl extensions.  
	int init_opengl_extensions(void)
	{
		//create dummy window
		COGLAppWindow wnd;
		RECT rect;
		ZeroMemory(&rect, sizeof rect);
		if (!wnd.Create(NULL, rect, L"", WS_OVERLAPPEDWINDOW))
			return 1;

		//fetch Device Context
		HDC hdc = wnd.GetDC();
		if (!hdc)
			return 2;

		//Load first available format
		ZeroMemory(&pfd, sizeof pfd);
		if (!SetPixelFormat(hdc, 1, &pfd))
			return 3;

		//Create OpenGL Context
		glrc = wglCreateContext(hdc);
		if (!glrc)
			return 4;

		//Make it current
		if (!wglMakeCurrent(hdc, glrc))
			return 5;

		//Load OpenGL and WGL extensions from the ICD
		auto ret = LoadWGLExtensions(hdc);
		if (ret != 0)
			return ret;

		//Discard
		wnd.ReleaseDC(hdc);
		wnd.DestroyWindow();

		return 0;
	}

	//Delete the OpenGL context and then destroy the window
	BOOL DestroyWindow()
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(glrc);
		CWindow::DestroyWindow();
		return 0;
	}

	//This function creates Modern OpenGL context compatible with 4.0 Specs
	int init_opengl()
	{
		//fetch Device Context
		HDC hdc = GetDC();

		//define Pixel format rquired
		int pixel_format_attribs[] =
		{
			WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
			WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
			WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,         32,
			WGL_DEPTH_BITS_ARB,         24,
			WGL_STENCIL_BITS_ARB,       8,
			0
		};

		//Check if it's available
		int pixel_format;
		UINT num_formats;
		wglChoosePixelFormatARB(hdc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
		if (!num_formats)
			return 2;

		//set it
		if (!SetPixelFormat(hdc, pixel_format, &pfd))
			return 3;

		int glver_attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, GL_MAJOR_VER,
			WGL_CONTEXT_MINOR_VERSION_ARB, GL_MINOR_VER,
			WGL_CONTEXT_PROFILE_MASK_ARB,  
			WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		//make context and core profile compatible
		glrc = wglCreateContextAttribsARB(hdc, 0, glver_attribs);
		if (!glrc)
			return 4;

		//make it current
		if (!wglMakeCurrent(hdc, glrc))
			return 5;

		ReleaseDC(hdc);
		return 0;
	}


	int  LoadWGLExtensions(HDC hdc)
	{
		if (gladLoaderLoadGL() < (GLAD_MAKE_VERSION(GL_MAJOR_VER, GL_MINOR_VER)))
			return 6;

		if (gladLoaderLoadWGL(hdc) < (GLAD_MAKE_VERSION(WGL_MAJOR_VER, WGL_MINOR_VER)))
			return 7;

		return 0;
	}


private:
	PIXELFORMATDESCRIPTOR pfd;
	HGLRC glrc;

	const int GL_MAJOR_VER = 4;
	const int GL_MINOR_VER = 0;

	const int WGL_MAJOR_VER = 1;
	const int WGL_MINOR_VER = 0;

};

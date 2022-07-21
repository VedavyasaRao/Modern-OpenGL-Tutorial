#include "GL/GL.h"
#include "GL/wglext.h"

void* GalogenGetProcAddress(const char *name);
PFNWGLCREATECONTEXTATTRIBSARBPROC funcptr_wglCreateContextAttribsARB;
PFNWGLCHOOSEPIXELFORMATARBPROC funcptr_wglChoosePixelFormatARB;

void LoadWGLExtensions()
{
	funcptr_wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)GalogenGetProcAddress("wglCreateContextAttribsARB");
	funcptr_wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)GalogenGetProcAddress("wglChoosePixelFormatARB");

}

HGLRC WINAPI wglCreateContextAttribsARB(HDC hDC, HGLRC hShareContext, const int *attribList)
{
	return funcptr_wglCreateContextAttribsARB(hDC, hShareContext, attribList);
}

BOOL WINAPI wglChoosePixelFormatARB(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats)
{
	return funcptr_wglChoosePixelFormatARB(hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);

}
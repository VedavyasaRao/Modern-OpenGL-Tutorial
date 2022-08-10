#pragma once

#include <algorithm>
#include <iterator>
#include <windows.h>
#include <gdiplus.h>
#include <gdiplusheaders.h>
#include "..\TextureUtil.h"
#include "textmesh.h"
#include "..\BaseGeometry.h"

using namespace Gdiplus;

class DrawTextUtil:public BaseGeometry
{
public:
	void Drawtext(PointF pt, WCHAR* text)
	{
		pgraphics->Clear(Color(0, 0, 0, 0));
		pgraphics->DrawString(text, wcslen(text), pfont, pt, ptextbrush);
		pbitmap->RotateFlip(RotateNoneFlipY);
		BitmapData bdata;
		pbitmap->LockBits(&Rect(0, 0, htwd, htwd), ImageLockModeRead, PixelFormat32bppARGB, &bdata);
		if (bfirst)
		{
			texutl.LoadTextTexture(bdata.Scan0, htwd);
			bfirst = false;
		}
		else
			texutl.LoadTextTextureImage(bdata.Scan0, htwd);

		pbitmap->UnlockBits(&bdata);
		Draw(false);
	}

	static void Startup()
	{
		GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}

	static void Shutdown()
	{
		GdiplusShutdown(gdiplusToken);
	}

	void Init(GLushort texunit, int htwd)
	{
		BaseGeometry::Init(new TextMesh());
		glDisable(GL_DEPTH_TEST);

		kount = mesh->GenerateVerticesData(false, VAOUtil::POS | VAOUtil::TEX, vaoutl);
		vaoutl.SetupVBO(0, VAOUtil::POS);
		vaoutl.SetupVBO(1, VAOUtil::TEX);
		vaoutl.unbindVAO();
		texutl.Init(texunit);

		this->htwd = htwd;
		pbitmap = new Bitmap(htwd, htwd, PixelFormat32bppARGB);
		pbitmap->SetResolution(res, res);
		pgraphics = Graphics::FromImage(pbitmap);
		pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintAntiAlias);
		bfirst = true;

	}

	void UpdateFontandColor(COLORREF clr, LPLOGFONTW lf)
	{
		Color gdipColor(255, 0, 0, 255);
		gdipColor.SetFromCOLORREF(clr);
		ptextbrush = new SolidBrush(gdipColor);
		HDC hdc = pgraphics->GetHDC();
		pfont = new Font(hdc, lf);
		pgraphics->ReleaseHDC(hdc);
	}


	void UpdateUniforms()
	{
		glDisable(GL_DEPTH_TEST);
		texutl.MakeActive(shader.GetUniformLocation("tex"));
		glUniformMatrix4fv(shader.GetUniformLocation("transform"), 1, GL_FALSE, glm::value_ptr(glm::mat4(GetTransformationMatrix())));
	}

	void Cleanup()
	{
		BaseGeometry::Cleanup();
		texutl.Cleanup();
	}

	string vertexShaderSource()
	{
		return R"(
		#version 330 core
		layout (location = 0) in vec3 vVertex;
		layout (location = 1) in vec2 vTexCrd;
		uniform mat4 transform;
		out vec2 FragTexCrd; 
		void main()
		{
			gl_Position = transform * vec4(vVertex, 1.0);
			FragTexCrd=vTexCrd;
		};
		)";
	}

	string fragmentShaderSource()
	{
		return R"(
		#version 330 core
		in vec2 FragTexCrd;
		out vec4 FragColor;
		uniform sampler2D tex;
		void main()
		{
			vec4 texColor = texture(tex, FragTexCrd);
			if(texColor.a < 0.1)
				discard;
			FragColor = texColor;
		};
		)";
	}


public:
	static ULONG_PTR gdiplusToken;

private:
	TextureUtil		texutl;
	Bitmap		*pbitmap;
	Graphics	*pgraphics;
	SolidBrush	*ptextbrush;
	int			htwd;
	float		res = 256.0;
	Font		*pfont;
	bool		bfirst = true;
};

ULONG_PTR DrawTextUtil::gdiplusToken = NULL;


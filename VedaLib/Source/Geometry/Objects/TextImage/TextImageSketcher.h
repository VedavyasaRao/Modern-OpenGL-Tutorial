#pragma once

#include <algorithm>
#include <map>
#include <iterator>
#include <memory>
#include <windows.h>
#include "..\..\Utils\Texture\TextureUtil.h"
#include "..\..\Mesh\textmesh.h"
#include "..\Base\BaseGeometry.h"

class TextImageSketcher :public BaseGeometry
{
public:

	void ClearCanvas(bool border=false)
	{
		pgraphics->Clear(Color(0, 0, 0, 0));
		if (border)
			pgraphics->DrawRectangle(ppen.get(), 0, 0, wd-1, ht-1);
	}

	void Drawtext(PointF pt, wstring text, HFONT hfont, StringFormat *fmt, Brush* pbrush)
	{
		Font* pfont = fontmap[hfont].get();
		if (pfont == nullptr)
		{
			HDC hdc = pgraphics->GetHDC();
			pfont = new Font(hdc, hfont);
			pgraphics->ReleaseHDC(hdc);
			fontmap[hfont].reset(pfont);
		}
		pgraphics->DrawString(text.c_str(), -1, pfont, pt, fmt, pbrush);
	}

	void Drawimage(Point pt, wstring filename, int clipWd, int clipHt)
	{
		Bitmap bitmap(filename.c_str());
		pgraphics->DrawImage(&bitmap, Rect(pt.X, pt.Y, clipWd, clipHt));
		pbitmap->RotateFlip(RotateNoneFlipY);
	}

	void DrawCanvas()
	{
		BitmapData bdata;
		pbitmap->LockBits(&Rect(0, 0, wd, ht), ImageLockModeRead, PixelFormat32bppARGB, &bdata);
		if (bfirst)
		{
			texutl.LoadTextTexture(bdata.Scan0, wd, ht);
			bfirst = false;
		}
		else
			texutl.LoadTextTextureImage(bdata.Scan0, wd, ht);

		pbitmap->UnlockBits(&bdata);
		Draw();
	}


	void Init(const TextureUtil::TexInfo& texinfo, int wd, int ht)
	{
		BaseGeometry::Init(new TextMesh());
		glDisable(GL_DEPTH_TEST);

		kount = mesh->GenerateVerticesData(VAOUtil::POS | VAOUtil::TEX, vaoutl);
		vaoutl.SetupVBO(0, VAOUtil::POS);
		vaoutl.SetupVBO(1, VAOUtil::TEX);
		vaoutl.unbindVAO();
		texutl.Init(texinfo);

		this->wd = wd;
		this->ht = ht;
		pbitmap.reset(new Bitmap(wd, ht, PixelFormat32bppARGB));
		pbitmap->SetResolution(res, res);
		pgraphics.reset(Graphics::FromImage(pbitmap.get()));
		pgraphics->SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);
		bfirst = true;
		ppen.reset(new Pen(Color(0, 0, 0)));
		ppen->SetDashStyle(DashStyleDash);
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
		pbitmap.release();
		pgraphics.release();
		ppen.release();
	}

	string vertexShaderSource()
	{
		return R"(
		#version 400 core
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
		#version 400 core
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
	TextureUtil						texutl;
	unique_ptr<Bitmap>				pbitmap;
	unique_ptr <Graphics>			pgraphics;
	map<HFONT, unique_ptr<Font>>	fontmap;
	int				ht,wd;
	float			res = 256.0;
	bool			bfirst = true;
	unique_ptr <Pen>	ppen;
};


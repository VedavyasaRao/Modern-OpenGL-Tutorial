#pragma once
#include "..\BaseGeometry.h"
#include "..\TextureUtil.h"
#include "QuadMesh.h"

//implements texturedcube
class Quad :public BaseGeometry
{
public:
	//initialize
	void Init(GLushort	texunit, const string& filename)
	{
		//create mesh and the window
		BaseGeometry::Init(new QuadMesh());
		texutl.Init(texunit);
		this->filename = filename;
	}

	void updateTextureMap(float texsize)
	{
		dynamic_cast<QuadMesh*>(mesh)->updateTextureMap(texsize);
	}

	void updateVertices(const std::vector < glm::vec3>& vertices)
	{
		dynamic_cast<QuadMesh*>(mesh)->updateVertices(vertices);
	}

	void GenerateVertices()
	{
		//generate VBOs for position and Texture coordinates
		kount = mesh->GenerateVerticesData( VAOUtil::POS | VAOUtil::TEX, vaoutl);

		//setup vertices
		vaoutl.SetupVBO(0, VAOUtil::POS);
		vaoutl.SetupVBO(1, VAOUtil::TEX);

		vaoutl.unbindVAO();

		//Load Texture from the file
		texutl.LoadTexture(filename);
	}

	//update uniforms
	void UpdateUniforms()
	{
		//pass builtin texture to the fragment shader 
		texutl.MakeActive(shader.GetUniformLocation("tex"));
		BaseGeometry::UpdateUniforms();
	}

	//release resources
	void Cleanup()
	{
		BaseGeometry::Cleanup();
		texutl.Cleanup();
	}

	//override
	virtual string vertexShaderSource()
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

	//override
	virtual string fragmentShaderSource()
	{
		return R"(
		#version 330 core
		in vec2 FragTexCrd;
		out vec4 FragColor;
		uniform sampler2D tex;
		void main()
		{
		   FragColor = texture(tex, FragTexCrd);
		};
		)";
	}

private:
	TextureUtil  texutl;
	std::string filename;
};


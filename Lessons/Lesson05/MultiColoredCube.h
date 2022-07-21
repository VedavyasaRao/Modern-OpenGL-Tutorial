#pragma once
#include "..\..\VOGLLib\Geometry\BaseGeometry.h"
#include "..\..\VOGLLib\Geometry\Cube\CubeMesh.h"
//Implements multi colored cube 
class MultiColoredCube :public BaseGeometry
{
public:
	//OpenGL initialization
	void Init()
	{
		//initialize opengl context
		BaseGeometry::Init(new CubeMesh());
		//generate vbo data
		kount = mesh->GenerateVerticesData(false, VAOUtil::POS | VAOUtil::CLR, vaoutl);
		//setup vertices
		vaoutl.SetupVBO(0, VAOUtil::POS);
		vaoutl.SetupVBO(1, VAOUtil::CLR);
		vaoutl.unbindVAO();
	}

	//override
	virtual string vertexShaderSource()
	{
		return R"(
		#version 330 core
		layout (location = 0) in vec3 vVertex;
		layout (location = 1) in vec3 vColor;
		out vec3 fcolor;

		uniform mat4 transform;

		void main()
		{
		   gl_Position =  transform * vec4(vVertex, 1.0);
		   fcolor = vColor;
		};
		)";
	}

	//override
	virtual string fragmentShaderSource()
	{
		return R"(
		#version 330 core
		in vec3 fcolor;
		out vec4 FragColor;
		void main()
		{
		   FragColor = vec4(fcolor,1.0);
		};
		)";
	}
};


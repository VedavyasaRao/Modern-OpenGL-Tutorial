#pragma once
#include "..\..\VOGLLib\Geometry\BaseGeometry.h"
#include "..\..\VOGLLib\Geometry\Cube\CubeMesh.h"

//implements EBO Indexed cube
class IndexedCube :public BaseGeometry
{
public:

	void Init()
	{
		//initialize
		BaseGeometry::Init(new CubeMesh());
		//update VBO data of the vertices
		mesh->GenerateVerticesData(true, VAOUtil::POS | VAOUtil::CLR, vaoutl);
		//setup vertex for Position
		vaoutl.SetupVBO(0, VAOUtil::POS);
		//setup vertex for Color
		vaoutl.SetupVBO(1, VAOUtil::CLR);
		//populate EBO indices
		kount = mesh->GenerateIndicesData(vaoutl);
		//bind VBO
		vaoutl.SetupEBO();

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


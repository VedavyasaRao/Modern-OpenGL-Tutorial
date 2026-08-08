#pragma once
#include "..\Base\BaseGeometry.h"
#include "..\Mesh\ObjFileMesh.h"
//Implements multi colored cube 
class MultiColoredCubeObj :public BaseGeometry
{
public:
	//OpenGL initialization
	void Init()
	{
		//initialize opengl context
		BaseGeometry::Init(new ObjectFileMesh(L"C:\\temp\\texturedknot.obj"));

		//generate vbo data
		kount = mesh->GenerateVerticesData(VAOUtil::POS | VAOUtil::CLR, vaoutl);

		//setup vertices
		vaoutl.SetupVBO(0, VAOUtil::POS);

		vaoutl.unbindVAO();
	}

	//override
	virtual string vertexShaderSource()
	{
		return R"(
		#version 400 core
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
		#version 400 core
		in vec3 fcolor;
		out vec4 FragColor;
		void main()
		{
		   FragColor = vec4(fcolor,1.0);
		};
		)";
	}
};


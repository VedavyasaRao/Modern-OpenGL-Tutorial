#pragma once
#include "..\..\VOGLLib\Geometry\BaseGeometry.h"
#include "..\..\VOGLLib\Geometry\Cube\CubeMesh.h"
//Implements singled colored cube 
class SingleColoredCube:public BaseGeometry
{
public:

	//Initialize
	void Init(glm::vec3 color)
	{
		//setup with a cube and Compile and link shaders
		BaseGeometry::Init(new CubeMesh());
		//assign the color
		this->color = color;
		//Generate VBO data
		kount = mesh->GenerateVerticesData(FALSE,VAOUtil::POS, vaoutl);
		//Enable single vertex
		vaoutl.SetupVBO(0, VAOUtil::POS);
		vaoutl.unbindVAO();
	}
	//Override to supply color of the cube
	void UpdateUniforms()
	{
		BaseGeometry::UpdateUniforms();
		glUniform3fv(shader.GetUniformLocation("color"), 1, glm::value_ptr(color));
	}

private:
	//override
	string vertexShaderSource()
	{
		return R"(
		#version 330 core
		layout (location = 0) in vec3 vVertex;

		uniform mat4 transform;
		uniform vec3 color;
		out vec3 cubecolor;

		void main()
		{
			gl_Position =  transform * vec4(vVertex, 1.0);
			cubecolor = color;
		};
		)";
	}
	//override
	string fragmentShaderSource()
	{
		return R"(
		#version 330 core
		in vec3 cubecolor;
		out vec4 FragColor;
		void main()
		{
		   FragColor = vec4(cubecolor,1);
		};
		)";
	}

	glm::vec3 color;
};


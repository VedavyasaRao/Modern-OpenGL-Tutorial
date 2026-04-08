#pragma once
#include "TexturedCube.h"


__declspec(dllexport) class BlinnPhongLightedTexCube :public TexturedCube
{
public:
	void Init()
	{
		TexturedCube::Init(0,R"(..\resources\textures\bricks2.jpg)");

		vaoutl.bindVAO();
		kount = mesh->GenerateVertices(VAOUtil::NOR, vaoutl);
		vaoutl.SetupAttribute(2, VAOUtil::NOR);
		vaoutl.unbindVAO();
	}

	void UpdateUniforms()
	{
		glUniform3fv(shader.GetUniformLocation("cameraPosition"), 1, glm::value_ptr(cameraPosition));
		light.Updateshader(shader);
		TexturedCube::UpdateUniforms();
	}


	virtual string vertexShaderSource()
	{
		return R"(
		#version 330 core
		layout (location = 0) in vec3 vVertex;
		layout (location = 1) in vec2 vTexCrd;
		layout (location = 2) in vec3 vNormal;

		uniform vec4 lightPosition;
		uniform mat4 transform;

		varying vec3 L, N, E;
		out vec2 fTexCrd;

		void main()
		{
			vec3 pos = (transform * vec4(vVertex,1)).xyz;
			vec3 lightPos = (lightPosition).xyz;

			L = normalize(lightPos - pos);
			N = normalize((transform * vec4(vNormal,1)).xyz);
			E = -normalize(pos);

			gl_Position = transform * vec4(vVertex,1);
			fTexCrd = vTexCrd;
		}
		)";
	}

	virtual string fragmentShaderSource()
	{
		return R"(
		#version 330 core

		in vec2 fTexCrd;

		precision mediump float;
		varying vec3 L, N, E;

		uniform vec4 ambientProduct;
		uniform vec4 diffuseProduct;
		uniform vec4 specularProduct;
		uniform float shininess;
		uniform sampler2D tex;

		void main()
		{
			vec4 diffuse = max(dot(L, N), 0.0) * diffuseProduct;
			vec3 H = normalize(L+E);
			vec4 specular = pow(max(dot(N, H), 0.0), shininess) * specularProduct;

			if (dot(L, N) < 0.0)
				specular = vec4(0.0, 0.0, 0.0, 1.0);

			vec4 fColor = ambientProduct + diffuse + specular;
			fColor.a = 1.0;

			gl_FragColor = fColor + texture(tex, fTexCrd);
	    }		
		)";

	}

private:
	BlinnPhongLightingUtil light;
	glm::vec3 cameraPosition;
};


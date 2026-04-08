#pragma once
#include "TexturedCube.h"


__declspec(dllexport) class PhongLightedTexCube:public TexturedCube
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
		light.light.position = glm::vec3(0.0f, 3.0f, -9.0f);
		cameraPosition = glm::vec3(2.0f, 0.0f, -4.0f);
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

		uniform mat4 transform;

		out vec2 FragTexCrd; 
		out vec3 FragVertex; 
		out vec3 FragNormal; 

		void main()
		{
			gl_Position = transform * vec4(vVertex, 1.0);
			FragVertex=vVertex;
			FragTexCrd=vTexCrd;
			FragNormal=vNormal;
		};
		)";
	}

	virtual string fragmentShaderSource()
	{
		return R"(
		#version 330 core
		in vec2 FragTexCrd; 
		in vec3 FragVertex; 
		in vec3 FragNormal; 

		out vec4 FragColor;
		uniform mat4 transform;
		uniform vec3 cameraPosition;
		uniform sampler2D tex;
		uniform struct Light
		{
		   vec3 position;
		   vec3 intensities; 
		   float attenuation;
		   float ambientCoefficient;
		} light;

		uniform struct Material 
		{
			float Shininess;
			vec3 SpecularColor;
		} material;

		void main()
		{
			vec3 normal = normalize(transpose(inverse(mat3(transform))) * FragNormal);
			vec3 surfacePos = vec3(transform * vec4(FragVertex, 1));
			vec4 surfaceColor = texture(tex, FragTexCrd);
			vec3 surfaceToLight = normalize(light.position - surfacePos);
			vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);
    
			//ambient
			vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;

			//diffuse
			float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
			vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;
    
			//specular
			float specularCoefficient = 0.0;
			if(diffuseCoefficient > 0.0)
				specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), material.Shininess);
			vec3 specular = specularCoefficient * material.SpecularColor * light.intensities;
    
			//attenuation
			float distanceToLight = length(light.position - surfacePos);
			float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

			//linear color (color before gamma correction)
			vec3 linearColor = ambient + attenuation*(diffuse + specular);
    
			//final color (after gamma correction)
			vec3 gamma = vec3(1.0/2.2);
			FragColor = vec4(pow(linearColor, gamma), surfaceColor.a);

		};
		)";

	}

private:
	PhongLightingUtil light;
	glm::vec3 cameraPosition;
};


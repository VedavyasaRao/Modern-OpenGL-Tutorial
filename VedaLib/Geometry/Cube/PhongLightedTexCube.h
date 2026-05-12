#pragma once
#include "TexturedCube.h"

class PhongLightedTexCube:public TexturedCube
{
public:
	void Init(bool blinn, GLushort	texunit, const string& filename)
	{
		light.light.blinn = blinn;
		TexturedCube::Init(0, filename);

		vaoutl.bindVAO();
		kount = mesh->GenerateVerticesData(VAOUtil::NOR, vaoutl);

		vaoutl.SetupVBO(2, VAOUtil::NOR);

		vaoutl.unbindVAO();
	}

	void UpdateUniforms()
	{
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
		uniform vec3 viewerPosition;
		uniform sampler2D tex;
		uniform bool blinn;

		uniform struct Light
		{
		   vec3 position;
		   vec3 intensities; 
		   float attenuation;
		   float ambientCoefficient;
			float diffuseCoefficient;
			float specularCoefficient;
		} light;

		uniform struct Material 
		{
			float Shininess;
			vec3 ambientColor;
			vec3 diffuseColor;
			vec3 SpecularColor;
		} material;

		void main()
		{
			vec3 normal = normalize(transpose(inverse(mat3(transform))) * FragNormal);
			vec3 surfacePos = vec3(transform * vec4(FragVertex, 1));
			vec4 surfaceColor = texture(tex, FragTexCrd);
			vec3 surfaceToLight = normalize(light.position - surfacePos);
			vec3 surfaceToViewer = normalize(viewerPosition - surfacePos);
    
			//ambient
			vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * material.ambientColor;

			//diffuse
			float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
			vec3 diffuse = light.diffuseCoefficient * diffuseCoefficient * surfaceColor.rgb * material.diffuseColor;;
    
			//specular
			float specularCoefficient = 0.0;
			if(diffuseCoefficient > 0.0)
			{
				if (blinn)
					specularCoefficient = pow(max(0.0, dot(normalize(surfaceToLight + surfaceToViewer),normal)), material.Shininess);
				else
					specularCoefficient = pow(max(0.0, dot(surfaceToViewer, reflect(-surfaceToLight, normal))), material.Shininess);
			}
			vec3 specular = light.specularCoefficient * specularCoefficient * material.SpecularColor;
	
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

	

public:
	PhongLightingUtil light;
};


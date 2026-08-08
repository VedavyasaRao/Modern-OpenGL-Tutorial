#pragma once
#include "..\Base\BaseGeometry.h"
#include "..\..\Utils\Texture\TextureUtil.h"
#include "..\..\Mesh\GenericObjMesh.h"
#include <regex>


//Implements singled colored cube 
class GenericObj :public BaseGeometry
{
public:
	void Init(int idx, const LightSrcInfo& lightsrc, GenericParser* pobjparser)
	{
		this->idx = idx;
		this->pobjparser = pobjparser;
		auto& aimesh = pobjparser->getmesh(idx);
		auto& mat = pobjparser->getmat4mesh(idx);

		light.lightsrc = lightsrc;
		light.mat = mat;

		auto m = new GenericObjMesh(idx, this->pobjparser);
		hastexture = pobjparser->hastexture(idx);

		BaseGeometry::Init(m);

		uint ptn = VAOUtil::POS | (hastexture ? VAOUtil::TEX : 0) | VAOUtil::NOR;

		//Generate VBO data
		kount = mesh->GenerateVerticesData(ptn, vaoutl);

		//Enable single vertex
		vaoutl.SetupVBO(0, VAOUtil::POS);
		if (hastexture)
			vaoutl.SetupVBO(1, VAOUtil::TEX);
		vaoutl.SetupVBO(2, VAOUtil::NOR);

		vaoutl.unbindVAO();

		if (hastexture)
		{
			texutl.Init(pobjparser->diffusetxtmap[pobjparser->matltextmap[&mat]]);
			texutl.LoadTexture();
		}

	}

	//Override to supply color of the cube
	void UpdateUniforms()
	{
		BaseGeometry::UpdateUniforms();
		if (hastexture)
			texutl.MakeActive(shader.GetUniformLocation("tex"));

		light.UpdateUniforms(shader);
	}



private:
	//override
	virtual string vertexShaderSource()
	{
		return  R"(
	#version 400 core
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
		FragTexCrd=vTexCrd;
		FragVertex=vVertex;
		FragNormal=vNormal;
		//VtxIdx = gl_VertexID;
	};
	)";

	}

	//override
	virtual string fragmentShaderSource()
	{
		auto s = (light.lightsrc.src != LightSourceType::None) ? fragmentShaderSourcewithlight() : fragmentShaderSourcewithoutlight();
		regex target("\\$1");
		if (hastexture)
			s = regex_replace(s, target, "");
		else
			s = regex_replace(s, target, "//");

		return s;
	}

	string fragmentShaderSourcewithlight()
	{
		return R"(
		#version 400 core
		in vec2 FragTexCrd; 
		in vec3 FragVertex; 
		in vec3 FragNormal; 

		out vec4 FragColor;
		uniform mat4 transform;
		uniform vec3 viewerPosition;
		uniform sampler2D tex;

		uniform struct Light
		{
		   vec3 position;
		   float ambientCoefficient;
			float diffuseCoefficient;
			float specularCoefficient;
			vec3 ambientColor;
			vec3 diffuseColor;
			vec3 SpecularColor;
			bool blinn;
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
			vec4 surfaceColor = vec4(material.diffuseColor,1.0);
			$1 surfaceColor = texture(tex, FragTexCrd);
			vec3 surfaceToLight = normalize(light.position - surfacePos);
			vec3 surfaceToViewer = normalize(viewerPosition - surfacePos);
    
			//ambient
			vec3 ambient = light.ambientCoefficient * material.ambientColor;   

			//diffuse
			float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
			vec3 diffuse = diffuseCoefficient * surfaceColor.rgb;
    
			//specular
			float specularCoefficient = 0.0;
			if(diffuseCoefficient > 0.0)
			{
				if (light.blinn)
					specularCoefficient = pow(max(0.0, dot(normalize(surfaceToLight + surfaceToViewer),normal)), material.Shininess);
				else
					specularCoefficient = pow(max(0.0, dot(surfaceToViewer, reflect(-surfaceToLight, normal))), material.Shininess);
			}
			vec3 specular = light.SpecularColor * specularCoefficient * material.SpecularColor;
	
			vec3 linearColor = ambient + diffuse + specular;
			FragColor = vec4(linearColor, 1.0);
		};
		)";
	}


	string fragmentShaderSourcewithoutlight()
	{
		return
		R"(
		#version 400 core
		in vec2 FragTexCrd; 
		in vec3 FragVertex; 
		in vec3 FragNormal; 

		out vec4 FragColor;
		uniform mat4 transform;
		uniform vec3 viewerPosition;
		uniform sampler2D tex;

		uniform struct Light
		{
		   vec3 position;
		   float ambientCoefficient;
			float diffuseCoefficient;
			float specularCoefficient;
			vec3 ambientColor;
			vec3 diffuseColor;
			vec3 SpecularColor;
			bool blinn;
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
			vec4 surfaceColor = vec4(material.diffuseColor,1.0);
			$1 surfaceColor = texture(tex, FragTexCrd);
			FragColor = surfaceColor;
		};
		)";
	}

private:
	TextureUtil		texutl;
	LightingUtil	light;
	bool hastexture = false;
	GenericParser* pobjparser;
	int idx;
};


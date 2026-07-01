#pragma once
#include "..\Base\BaseGeometry.h"
#include "..\..\Utils\Texture\TextureUtil.h"
#include "..\..\Mesh\AssimpModelMesh.h"
#include "..\..\Mesh\WFObjMesh.h"
#include <regex>

//Implements singled colored cube 
class AssimpModel :public BaseGeometry
{
public:
	class  WFOBJInfo;

	//Initialize
	void Init(const WFOBJInfo& shapeinf)
	{
		this->shapeinf = shapeinf;

		auto m2 = new AssimpModelMesh(this->shapeinf.objfilename);
		auto m = new WFObjMesh(this->shapeinf.objfilename, this->shapeinf.mtlfilename);
		hastexture = (((m->texture_count() != 0)) && this->shapeinf.txtinf);

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
			texutl.Init(this->shapeinf.txtinf);
			texutl.LoadTexture();
		}
	}

	void UpdateUniformsLightMat(ShaderUtil& shader)
	{

		glUniform3fv(shader.GetUniformLocation("viewerPosition"), 1, glm::value_ptr(shapeinf.viewerpos));

		glUniform1f(shader.GetUniformLocation("light.ambientCoefficient"), shapeinf.light.ambientCoefficient);
		glUniform3fv(shader.GetUniformLocation("light.Position"), 1, glm::value_ptr(shapeinf.light.Position));
		glUniform3fv(shader.GetUniformLocation("light.Color"), 1, value_ptr(shapeinf.light.Color));


		auto& mat = ((WFObjMesh*)mesh)->matinfomap.begin()->second;
		glUniform3fv(shader.GetUniformLocation("material.ambientColor"), 1, glm::value_ptr(mat.ambientclr));
		glUniform3fv(shader.GetUniformLocation("material.diffuseColor"), 1, glm::value_ptr(mat.diffuseclr));
		glUniform3fv(shader.GetUniformLocation("material.SpecularColor"), 1, value_ptr(mat.specularclr));
		glUniform1f(shader.GetUniformLocation("material.Shininess"), mat.shininess);
	}


	//Override to supply color of the cube
	void UpdateUniforms()
	{
		BaseGeometry::UpdateUniforms();
		if (hastexture)
			texutl.MakeActive(shader.GetUniformLocation("tex"));

		UpdateUniformsLightMat(shader);
	}

	class WFOBJInfo
	{
	public:
		struct Light;

		WFOBJInfo() = default;
		WFOBJInfo(const string& objfilename, const string& mtlfilename, const TextureUtil::TexInfo& txtinf, vec3 viewerpos, const Light& light)
			:objfilename(objfilename), mtlfilename(mtlfilename), txtinf(txtinf), viewerpos(viewerpos), light(light){}

		struct Light
		{
			float ambientCoefficient;
			vec3 Position;
			vec3 Color;
		};

	private:
		string objfilename;
		string mtlfilename;
		TextureUtil::TexInfo txtinf;
		vec3 viewerpos;
		Light light;

		friend class AssimpModel;
	};

private:
	//override
	virtual string vertexShaderSource()
	{
		return  R"(
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
		string s = 
		R"(
		#version 330 core
		in vec2 FragTexCrd;
		in vec3 FragVertex; 
		in vec3 FragNormal; 

		out vec4 FragColor;

		uniform sampler2D tex;
		uniform mat4 transform;
		uniform vec3 viewerPosition;

		uniform struct Light
		{
			float ambientCoefficient;
			vec3 Position;
			vec3 Color;
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
			$1surfaceColor = texture(tex, FragTexCrd);
			vec3 normal = normalize(transpose(inverse(mat3(transform))) * FragNormal);
			vec3 surfacePos = vec3(transform * vec4(FragVertex, 1));
			vec3 surfaceToLight = normalize(light.Position - surfacePos);
			vec3 surfaceToViewer = normalize(viewerPosition - surfacePos);
    
			//ambient
			vec3 ambient = material.ambientColor * light.Color * light.ambientCoefficient;   

			//diffuse
			float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
			vec3 diffuse = diffuseCoefficient *  vec3(surfaceColor);
    
			//specular
			float specularCoefficient = 0.0;
			if(diffuseCoefficient > 0.0)
			{
				specularCoefficient = pow(max(0.0, dot(normalize(surfaceToLight + surfaceToViewer),normal)), material.Shininess);
			}
			vec3 specular = specularCoefficient *  material.SpecularColor  * light.Color;

			vec3 linearColor = (ambient + diffuse + specular);
			FragColor = vec4(linearColor, 1.0);
			FragColor = surfaceColor;
		};
		)";

		regex target("\\$1");
		if (hastexture)
			s = regex_replace(s, target, "");
		else
			s = regex_replace(s, target, "//");

		return s;
	}


private:
	TextureUtil  texutl;
	bool hastexture=false;
	WFOBJInfo shapeinf;
};


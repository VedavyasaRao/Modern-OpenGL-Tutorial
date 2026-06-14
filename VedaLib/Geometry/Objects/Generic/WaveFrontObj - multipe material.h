#pragma once
#include "..\Base\BaseGeometry.h"
#include "..\..\Utils\Texture\TextureUtil.h"
#include "..\..\Mesh\WaveFrontObjMesh.h"
#include <regex>

//Implements singled colored cube 
class WaveFrontObj :public BaseGeometry
{
public:
	class  WFOBJInfo;

	//Initialize
	void Init(const WFOBJInfo& shapeinf)
	{

		auto m = new WFObjMesh(shapeinf.objfilename, shapeinf.mtlfilename);
		hastexture = (((m->texture_count() != 0)) && shapeinf.txtinf);

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
			texutl.Init(shapeinf.txtinf);
			texutl.LoadTexture();
		}
	}

	void UpdateUniformsLightMat(ShaderUtil& shader)
	{
		viewerpos = vec3(2.0, 0.0, -4.0);
		light.ambientCoefficient = 0.05f;
		light.Position = vec3(0.0, 3.0, -9.0);
		light.Color = vec3(1.0, 1.0, 1.0);

		int i = 0;
		for (auto& kv : ((WFObjMesh*)mesh)->matinfomap)
		{
			auto& mat = kv.second;
			material[i].idxstart = mat.range.first;
			material[i].idxend = mat.range.second;
			material[i].ambientColor = mat.ambientclr;
			material[i].diffuseColor = mat.diffuseclr;
			material[i].SpecularColor = mat.specularclr;
			material[i++].Shininess = mat.shininess;
		}

		

		glUniform3fv(shader.GetUniformLocation("viewerPosition"), 1, glm::value_ptr(viewerpos));

		glUniform1f(shader.GetUniformLocation("light.ambientCoefficient"), light.ambientCoefficient);
		glUniform3fv(shader.GetUniformLocation("light.Position"), 1, glm::value_ptr(light.Position));
		glUniform3fv(shader.GetUniformLocation("light.Color"), 1, value_ptr(light.Color));

		i = 0;
		for (auto& kv : ((WFObjMesh*)mesh)->matinfomap)
		{
			auto mat = "material[" + to_string(i) + "].";
			glUniform3fv(shader.GetUniformLocation(mat + "ambientColor"), 1, glm::value_ptr(material[i].ambientColor));
			glUniform3fv(shader.GetUniformLocation(mat + "diffuseColor"), 1, glm::value_ptr(material[i].diffuseColor));
			glUniform3fv(shader.GetUniformLocation(mat + "SpecularColor"), 1, value_ptr(material[i].SpecularColor));
			glUniform1f(shader.GetUniformLocation(mat + "Shininess"), material[i++].Shininess);
		}
		glUniform1i(shader.GetUniformLocation("matlen"), i);
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
		WFOBJInfo() = default;
		WFOBJInfo(const string& objfilename, const string& mtlfilename, const TextureUtil::TexInfo txtinf) :objfilename(objfilename), mtlfilename(mtlfilename), txtinf(txtinf) {}
	
	private:
		string objfilename;
		string mtlfilename;
		TextureUtil::TexInfo txtinf;
		friend class WaveFrontObj;
	};

private:
	//override
	virtual string vertexShaderSource()
	{
		string s =  R"(
		#version 330 core
		layout (location = 0) in vec3 vVertex;
		layout (location = 1) in vec2 vTexCrd;
		layout (location = 2) in vec3 vNormal;


		uniform mat4 transform;
		out vec2 FragTexCrd; 
		out vec3 FragVertex; 
		out vec3 FragNormal; 
		flat out int  VtxIdx; 

		void main()
		{
			gl_Position = transform * vec4(vVertex, 1.0);
			FragTexCrd=vTexCrd;
			FragVertex=vVertex;
			FragNormal=vNormal;
			VtxIdx = gl_VertexID;
		};
		)";
		
		regex target("\\$1");
		if (hastexture)
			s = regex_replace(s, target, "");
		else
			s = regex_replace(s, target, "//");

		return s;
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
		flat in int  VtxIdx; 

		out vec4 FragColor;

		$1uniform sampler2D tex;
		uniform mat4 transform;
		uniform vec3 viewerPosition;
		uniform int matlen;

		uniform struct Light
		{
			float ambientCoefficient;
			vec3 Position;
			vec3 Color;
		} light;

		uniform struct Material 
		{
			int idxstart;
			int idxend;
			float Shininess;
			vec3 ambientColor;
			vec3 diffuseColor;
			vec3 SpecularColor;
		} material[5];

		void main()
		{
			int  MatIdx = 0;
            for (int i=0; i < matlen; ++i)
			{
				if (VtxIdx >= material[i].idxstart && VtxIdx <= material[i].idxend)
					MatIdx = i;
			}
			vec4 surfaceColor = vec4(material[MatIdx].diffuseColor,1.0);
			$1surfaceColor = texture(tex, FragTexCrd);
			vec3 normal = normalize(transpose(inverse(mat3(transform))) * FragNormal);
			vec3 surfacePos = vec3(transform * vec4(FragVertex, 1));
			vec3 surfaceToLight = normalize(light.Position - surfacePos);
			vec3 surfaceToViewer = normalize(viewerPosition - surfacePos);
    
			//ambient
			vec3 ambient = material[MatIdx].ambientColor * light.Color * light.ambientCoefficient;   

			//diffuse
			float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
			vec3 diffuse = diffuseCoefficient *  vec3(surfaceColor);
    
			//specular
			float specularCoefficient = 0.0;
			if(diffuseCoefficient > 0.0)
			{
				specularCoefficient = pow(max(0.0, dot(normalize(surfaceToLight + surfaceToViewer),normal)), material[MatIdx].Shininess);
			}
			vec3 specular = specularCoefficient *  material[MatIdx].SpecularColor  * light.Color;

			vec3 linearColor = (ambient + diffuse + specular);
			FragColor = vec4(linearColor, 1.0);
		};
		)";

		regex target("\\$1");
		if (hastexture)
			s = regex_replace(s, target, "");
		else
			s = regex_replace(s, target, "//");

		return s;
	}

	/*

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

// MTL Material Properties
uniform vec3 material.Ka;
uniform vec3 material.Kd;
uniform vec3 material.Ks;
uniform float material.shininess;

// Textures
uniform sampler2D material.map_Kd;
uniform bool has_map_Kd;

// Light & Camera Properties
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
	// Ambient
	vec3 ambient = material.Ka * lightColor;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * material.Kd * lightColor;

	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * material.Ks * lightColor;

	// Apply Diffuse Texture if it exists in the MTL
	vec4 finalColor = vec4(ambient + diffuse + specular, 1.0);
	if (has_map_Kd) {
		finalColor = texture(material.map_Kd, TexCoord) * vec4(ambient + diffuse + specular, 1.0);
	}

	FragColor = finalColor;
}
	}
*/


private:
	TextureUtil  texutl;
	bool hastexture=false;
	vec3 viewerpos;
	
	struct Light
	{
		float ambientCoefficient;
		vec3 Position;
		vec3 Color;
	} light;

	struct Material
	{
		int idxstart;
		int idxend;
		float Shininess;
		vec3 ambientColor;
		vec3 diffuseColor;
		vec3 SpecularColor;
	} material[5];
};


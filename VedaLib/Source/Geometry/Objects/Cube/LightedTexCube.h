#pragma once
#include "TexturedCube.h"

class LightedTexCube:public TexturedCube
{
public:
	void Init(GLushort	texunit, const string& filename)
	{
		TexturedCube::Init(TextureUtil::TexInfo(0, filename));

		vaoutl.bindVAO();
		kount = mesh->GenerateVerticesData(VAOUtil::NOR, vaoutl);

		vaoutl.SetupVBO(2, VAOUtil::NOR);

		vaoutl.unbindVAO();
	}

	void UpdateUniforms()
	{
		light.UpdateUniforms(shader);
		TexturedCube::UpdateUniforms();
	}

	wstring getangless()
	{
		return L" (P=" + to_wstring(MM.Pitch) + L" Y=" + to_wstring(MM.Yaw) + L" R=" + to_wstring(MM.Roll) + L")";
	}

	virtual string vertexShaderSource()
	{
		return R"(
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
			FragVertex=vVertex;
			FragTexCrd=vTexCrd;
			FragNormal=vNormal;
		};
		)";
	}

	virtual string fragmentShaderSource()
	{
		auto src = light.lightsrc.src;
		if (src == LightingUtil::Basic)
			return fragmentShaderSourceBasic();
		else if (src == LightingUtil::Directional)
			return fragmentShaderSourceDirectional();
		else if (src == LightingUtil::Point)
			return fragmentShaderSourcePoint();
		else if (src == LightingUtil::Spot)
			return fragmentShaderSourceSpot();
		return "";
	}

	virtual string fragmentShaderSourceSpot()
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
			vec3 direction;
			float ambientCoefficient;
			float diffuseCoefficient;
			float specularCoefficient;
			vec3 ambientColor;
			vec3 diffuseColor;
			vec3 SpecularColor;
			bool blinn;

			float cosInnerCone;
			float cosOuterCone;

			float constant;
			float linear;
			float exponent;
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
			vec3 spotDir  = normalize(light.direction);
   
			//ambient
			vec3 ambient = light.ambientColor * light.ambientCoefficient * material.ambientColor * surfaceColor.rgb;   

			//diffuse
			float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
			vec3 diffuse = light.diffuseColor * light.diffuseCoefficient * diffuseCoefficient * surfaceColor.rgb;
    
			//specular
			float specularCoefficient = 0.0;
			if(diffuseCoefficient > 0.0)
			{
				if (light.blinn)
					specularCoefficient = pow(max(0.0, dot(normalize(surfaceToLight + surfaceToViewer),normal)), material.Shininess);
				else
					specularCoefficient = pow(max(0.0, dot(surfaceToViewer, reflect(-surfaceToLight, normal))), material.Shininess);
			}

			vec3 specular = light.SpecularColor * light.specularCoefficient * specularCoefficient * material.SpecularColor;
	
			// Attenuation using Kc, Kl, Kq ---------------------------------------------------------
			float d = length(light.position - FragVertex);  // distance to light
			float attenuation = 1.0f / (light.constant + light.linear * d + light.exponent * (d * d));

			float cosDir = dot(-surfaceToLight, spotDir);  // angle between the lights direction vector and spotlights direction vector
			float spotIntensity = smoothstep(light.cosOuterCone, light.cosInnerCone, cosDir);

			vec3 linearColor = ambient + diffuse * attenuation * spotIntensity + specular * attenuation  * spotIntensity ;
			FragColor = vec4(linearColor, 1.0);
		};
		)";
	}

	virtual string fragmentShaderSourcePoint()
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

			float constant;
			float linear;
			float exponent;
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
			vec3 ambient = light.ambientColor * light.ambientCoefficient * material.ambientColor * surfaceColor.rgb;   

			//diffuse
			float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
			vec3 diffuse = light.diffuseColor * light.diffuseCoefficient * diffuseCoefficient * surfaceColor.rgb;
    
			//specular
			float specularCoefficient = 0.0;
			if(diffuseCoefficient > 0.0)
			{
				if (light.blinn)
					specularCoefficient = pow(max(0.0, dot(normalize(surfaceToLight + surfaceToViewer),normal)), material.Shininess);
				else
					specularCoefficient = pow(max(0.0, dot(surfaceToViewer, reflect(-surfaceToLight, normal))), material.Shininess);
			}
			vec3 specular = light.SpecularColor * light.specularCoefficient * specularCoefficient * material.SpecularColor;
	
			// Attenuation using Kc, Kl, Kq ---------------------------------------------------------
			float d = length(light.position - FragVertex);  // distance to light
			float attenuation = 1.0f / (light.constant + light.linear * d + light.exponent * (d * d));

			vec3 linearColor = ambient + diffuse * attenuation + specular * attenuation ;
			FragColor = vec4(linearColor, 1.0);
		};
		)";
	}

	virtual string fragmentShaderSourceDirectional()
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
			vec3 direction;
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
			vec4 surfaceColor = texture(tex, FragTexCrd);
			vec3 surfaceToLight = normalize(-light.direction);
			vec3 surfaceToViewer = normalize(viewerPosition - surfacePos);
    
			//ambient
			vec3 ambient = light.ambientColor * light.ambientCoefficient * material.ambientColor * surfaceColor.rgb;   

			//diffuse
			float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
			vec3 diffuse = light.diffuseColor * light.diffuseCoefficient * diffuseCoefficient * surfaceColor.rgb;
    
			//specular
			float specularCoefficient = 0.0;
			if(diffuseCoefficient > 0.0)
			{
				if (light.blinn)
					specularCoefficient = pow(max(0.0, dot(normalize(surfaceToLight + surfaceToViewer),normal)), material.Shininess);
				else
					specularCoefficient = pow(max(0.0, dot(surfaceToViewer, reflect(-surfaceToLight, normal))), material.Shininess);
			}
			vec3 specular = light.SpecularColor * light.specularCoefficient * specularCoefficient * material.SpecularColor;
	
			vec3 linearColor = ambient + diffuse + specular;
			FragColor = vec4(linearColor, 1.0);
		};
		)";
	}

	virtual string fragmentShaderSourceBasic()
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
			vec4 surfaceColor = texture(tex, FragTexCrd);
			vec3 surfaceToLight = normalize(light.position - surfacePos);
			vec3 surfaceToViewer = normalize(viewerPosition - surfacePos);
    
			//ambient
			vec3 ambient = light.ambientColor * light.ambientCoefficient * material.ambientColor;   

			//diffuse
			float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
			vec3 diffuse = light.diffuseColor * light.diffuseCoefficient * diffuseCoefficient * surfaceColor.rgb;// * material.diffuseColor;
    
			//specular
			float specularCoefficient = 0.0;
			if(diffuseCoefficient > 0.0)
			{
				if (light.blinn)
					specularCoefficient = pow(max(0.0, dot(normalize(surfaceToLight + surfaceToViewer),normal)), material.Shininess);
				else
					specularCoefficient = pow(max(0.0, dot(surfaceToViewer, reflect(-surfaceToLight, normal))), material.Shininess);
			}
			vec3 specular = light.SpecularColor * light.specularCoefficient * specularCoefficient * material.SpecularColor;
	
			vec3 linearColor = ambient + diffuse + specular;
			FragColor = vec4(linearColor, 1.0);
		};
		)";
	}


public:
	LightingUtil	light;
};


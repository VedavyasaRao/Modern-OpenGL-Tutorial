#pragma once
/////////////////////PhongLightingUtil///////////////////////////////////
class PhongLightingUtil
{
public:
	void Updateshader(ShaderUtil& shader)
	{
		glUniform1f(shader.GetUniformLocation("material.Shininess"), material.Shininess);
		glUniform3fv(shader.GetUniformLocation("material.SpecularColor"), 1, glm::value_ptr(material.SpecularColor));

		glUniform3fv(shader.GetUniformLocation("light.position"), 1, glm::value_ptr(light.position));
		glUniform3fv(shader.GetUniformLocation("light.intensities"), 1, glm::value_ptr(light.intensities));
		glUniform1f(shader.GetUniformLocation("light.ambientCoefficient"), light.ambientCoefficient);
	}

public:
	struct Light
	{
		glm::vec3 position = glm::vec3(0.0f, 3.0f, -9.0f);
		glm::vec3 intensities = glm::vec3(1.0f, 1.0f, 1.0f);
		float attenuation = 0.2f;
		float ambientCoefficient = 0.005f;
	}light;

	struct Material
	{
		float Shininess = 100.0;
		glm::vec3 SpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	}material;
};
/////////////////////PhongLightingUtil///////////////////////////////////

/////////////////////BlinnPhongLightingUtil///////////////////////////////////
class BlinnPhongLightingUtil
{
public:
	void Updateshader(ShaderUtil& shader)
	{
		glUniform1f(shader.GetUniformLocation("shininess"), material.Shininess);
		glUniform4fv(shader.GetUniformLocation("lightPosition"), 1, value_ptr(light.Position));

		glUniform4fv(shader.GetUniformLocation("AmbientProduct"), 1, value_ptr(light.Ambient*material.Ambient));
		glUniform4fv(shader.GetUniformLocation("DiffuseProduct"), 1, value_ptr(light.Diffuse*material.Diffuse));
		glUniform4fv(shader.GetUniformLocation("SpecularProduct"), 1, value_ptr(light.Specular*material.Specular));
	}

public:
	struct
	{
		vec4 Position = vec4(0.0f, 3.0f, -9.0f, 1.0);
		vec4 Ambient = vec4(0.2, 0.2, 0.2, 1.0);
		vec4 Diffuse = vec4(1.0, 1.0, 1.0, 1.0);
		vec4 Specular = vec4(1.0, 1.0, 1.0, 1.0);
	}light;

	struct
	{
		vec4 Ambient = vec4(0.0, 1.0, 0.0, 1.0);
		vec4 Diffuse = vec4(0.4, 0.8, 0.4, 1.0);
		vec4 Specular = vec4(0.0, 0.4, 0.4, 1.0);
		float Shininess = 300.0;
	}material;
};
/////////////////////BlinnPhongLightingUtil///////////////////////////////////

#pragma once
/////////////////////PhongLightingUtil///////////////////////////////////
class PhongLightingUtil
{
public:

	void Updateshader(ShaderUtil& shader)
	{
		glUniform1f(shader.GetUniformLocation("material.Shininess"), material.Shininess);
		glUniform3fv(shader.GetUniformLocation("material.SpecularColor"), 1, value_ptr(material.specularColor));

		glUniform3fv(shader.GetUniformLocation("light.position"), 1, glm::value_ptr(light.position));
		glUniform1f(shader.GetUniformLocation("light.ambientCoefficient"), light.ambientCoefficient);

		glUniform3fv(shader.GetUniformLocation("viewerPosition"), 1, glm::value_ptr(viewerPosition));
		glUniform1f(shader.GetUniformLocation("light.attenuation"), light.attenuation);
		glUniform1f(shader.GetUniformLocation("light.diffuseCoefficient"), light.diffuseCoefficient);
		glUniform1f(shader.GetUniformLocation("light.specularCoefficient"), light.specularCoefficient);
		glUniform3fv(shader.GetUniformLocation("material.ambientColor"), 1, glm::value_ptr(material.ambientColor));
		glUniform3fv(shader.GetUniformLocation("material.diffuseColor"), 1, glm::value_ptr(material.diffuseColor));
		glUniform1i(shader.GetUniformLocation("blinn"),  light.blinn);
	}

public:
	struct Light
	{
		vec3 position = vec3(0.0f, 3.0f, -9.0f);
		float attenuation = 0.02f;
		bool blinn = false;
		float ambientCoefficient = 0.005f;
		float diffuseCoefficient = 0.005f;
		float specularCoefficient = 0.005f;
	}light;

	struct Material
	{
		vec3 ambientColor = vec3(0.0, 1.0, 0.0);
		vec3 diffuseColor = vec3(0.4, 0.8, 0.4);
		vec3 specularColor = vec3(1.0f, 1.0f, 1.0f);
		float Shininess = 3000.0;
	}material;

	vec3 viewerPosition = vec3(2.0f, 0.0f, -4.0f);
};
/////////////////////PhongLightingUtil///////////////////////////////////


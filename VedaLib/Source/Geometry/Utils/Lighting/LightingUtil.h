#pragma once

enum LightSourceType { None=0, Basic = 1, Directional = 2, Point = 3, Spot = 4 };


struct MaterialInfo
{
	string name;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float Shininess;
};

struct LightSrcInfo
{
	LightSourceType src = LightSourceType::None;

	string name;
	float ambientCoefficient;
	vec3 ambientColor;

	float diffuseCoefficient;
	vec3 diffuseColor;

	vec3 specularColor;
	float specularCoefficient;

	vec3 viewerPosition;
	bool blinn;

	vec3 position;
	vec3 direction;

	float attconstant;
	float attlinear;
	float attquadratic;

	float spotlightinner;
	float spotlightouter;
};


class LightingUtil
{
public:
	
	void UpdateUniforms(ShaderUtil& shader)
	{
		auto src = lightsrc.src;
		if (src == LightSourceType::Basic)
			UpdateUniformsBasic(shader);
		else if (src == Directional)
			UpdateUniformsDirectional(shader);
		else if (src == LightSourceType::Point)
			UpdateUniformsPoint(shader);
		else if (src == LightSourceType::Spot)
			UpdateUniformsSpot(shader);
	}

private:
	void UpdateUniformsBasic(ShaderUtil& shader)
	{
		glUniform3fv(shader.GetUniformLocation("light.position"), 1, glm::value_ptr(lightsrc.position));
		UpdateUniformsLightMat(shader);
	}

	void UpdateUniformsDirectional(ShaderUtil& shader)
	{
		glUniform3fv(shader.GetUniformLocation("light.direction"), 1, glm::value_ptr(lightsrc.direction));
		UpdateUniformsLightMat(shader);
	}

	void UpdateUniformsPoint(ShaderUtil& shader)
	{
		glUniform3fv(shader.GetUniformLocation("light.position"), 1, glm::value_ptr(lightsrc.position));
		glUniform1f(shader.GetUniformLocation("light.constant"), lightsrc.attconstant);
		glUniform1f(shader.GetUniformLocation("light.linear"), lightsrc.attlinear);
		glUniform1f(shader.GetUniformLocation("light.exponent"), lightsrc.attquadratic);
		UpdateUniformsLightMat(shader);
	}

	void UpdateUniformsSpot(ShaderUtil& shader)
	{
		glUniform3fv(shader.GetUniformLocation("light.position"), 1, glm::value_ptr(lightsrc.position));
		glUniform3fv(shader.GetUniformLocation("light.direction"), 1, glm::value_ptr(lightsrc.direction));
		glUniform1f(shader.GetUniformLocation("light.constant"), lightsrc.attconstant);
		glUniform1f(shader.GetUniformLocation("light.linear"), lightsrc.attlinear);
		glUniform1f(shader.GetUniformLocation("light.exponent"), lightsrc.attquadratic);
		glUniform1f(shader.GetUniformLocation("light.cosInnerCone"), glm::cos(glm::radians(lightsrc.spotlightinner)));
		glUniform1f(shader.GetUniformLocation("light.cosOuterCone"), glm::cos(glm::radians(lightsrc.spotlightouter)));
		UpdateUniformsLightMat(shader);
	}

	void UpdateUniformsLightMat(ShaderUtil& shader)
	{
		glUniform3fv(shader.GetUniformLocation("viewerPosition"), 1, glm::value_ptr(lightsrc.viewerPosition));
		glUniform1i(shader.GetUniformLocation("light.blinn"), lightsrc.blinn);
		glUniform1f(shader.GetUniformLocation("light.ambientCoefficient"), lightsrc.ambientCoefficient);
		glUniform1f(shader.GetUniformLocation("light.diffuseCoefficient"), lightsrc.diffuseCoefficient);
		glUniform1f(shader.GetUniformLocation("light.specularCoefficient"), lightsrc.specularCoefficient);

		glUniform3fv(shader.GetUniformLocation("light.ambientColor"), 1, glm::value_ptr(lightsrc.ambientColor));
		glUniform3fv(shader.GetUniformLocation("light.diffuseColor"), 1, glm::value_ptr(lightsrc.diffuseColor));
		glUniform3fv(shader.GetUniformLocation("light.SpecularColor"), 1, value_ptr(lightsrc.specularColor));

		glUniform3fv(shader.GetUniformLocation("material.ambientColor"), 1, glm::value_ptr(mat.ambientColor));
		glUniform3fv(shader.GetUniformLocation("material.diffuseColor"), 1, glm::value_ptr(mat.diffuseColor));
		glUniform3fv(shader.GetUniformLocation("material.SpecularColor"), 1, value_ptr(mat.specularColor));
		glUniform1f(shader.GetUniformLocation("material.Shininess"), mat.Shininess);
	}

public:
	MaterialInfo	mat;
	LightSrcInfo	lightsrc;
};


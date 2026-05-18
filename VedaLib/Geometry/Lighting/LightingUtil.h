#pragma once
/////////////////////PhongLightingUtil///////////////////////////////////
class LightingUtil
{
public:
	struct  LightingUtil::LightSrc;
	struct LightingUtil::CommonLightMaterial;

	enum LightSourceType { Basic = 0, Directional = 1, Point = 2, Spot = 3 };

	void UpdateUniforms(ShaderUtil& shader)
	{
		auto src = lightsrc.src;
		if (src == Basic)
			UpdateUniformsBasic(shader);
		else if (src == Directional)
			UpdateUniformsDirectional(shader);
		else if (src == Point)
			UpdateUniformsPoint(shader);
		else if (src == Spot)
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
		glUniform3fv(shader.GetUniformLocation("viewerPosition"), 1, glm::value_ptr(commonlightmat.light.viewerPosition));
		glUniform1i(shader.GetUniformLocation("light.blinn"), commonlightmat.light.blinn);
		glUniform1f(shader.GetUniformLocation("light.ambientCoefficient"), commonlightmat.light.ambientCoefficient);
		glUniform1f(shader.GetUniformLocation("light.diffuseCoefficient"), commonlightmat.light.diffuseCoefficient);
		glUniform1f(shader.GetUniformLocation("light.specularCoefficient"), commonlightmat.light.specularCoefficient);

		glUniform3fv(shader.GetUniformLocation("light.ambientColor"), 1, glm::value_ptr(commonlightmat.light.ambientColor));
		glUniform3fv(shader.GetUniformLocation("light.diffuseColor"), 1, glm::value_ptr(commonlightmat.light.diffuseColor));
		glUniform3fv(shader.GetUniformLocation("light.SpecularColor"), 1, value_ptr(commonlightmat.light.specularColor));

		glUniform3fv(shader.GetUniformLocation("material.ambientColor"), 1, glm::value_ptr(commonlightmat.mat.ambientColor));
		glUniform3fv(shader.GetUniformLocation("material.diffuseColor"), 1, glm::value_ptr(commonlightmat.mat.diffuseColor));
		glUniform3fv(shader.GetUniformLocation("material.SpecularColor"), 1, value_ptr(commonlightmat.mat.specularColor));
		glUniform1f(shader.GetUniformLocation("material.Shininess"), commonlightmat.mat.Shininess);
	}

public:
	struct CommonLightMaterial
	{
		struct LightCommon
		{
			float ambientCoefficient;
			float diffuseCoefficient;
			float specularCoefficient;

			vec3 ambientColor;
			vec3 diffuseColor;
			vec3 specularColor;

			vec3 viewerPosition;
			bool blinn;
		}light;
		struct Material
		{
			vec3 ambientColor;
			vec3 diffuseColor;
			vec3 specularColor;
			float Shininess;
		}mat;
	} commonlightmat;

	struct LightSrc
	{
		LightSourceType src;
		vec3 position;
		vec3 direction;

		float attconstant;
		float attlinear;
		float attquadratic;

		float spotlightinner;
		float spotlightouter;
	}lightsrc;
};
/////////////////////PhongLightingUtil///////////////////////////////////


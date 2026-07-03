#pragma once
#include "..\..\Mesh\Base\GeometryMesh.h"
#include "..\..\..\Canvas\Camera\Base\CameraData.h"
#include "..\..\Utils\VAO\VAOUtil.h"
#include "..\..\Utils\Shader\ShaderUtil.h"

/////////////////////BaseGeometry///////////////////////////////////
class BaseGeometry
{
public:
	virtual void Init(IGeometryMesh *pmesh)
	{
		shader.LoadFromString(GL_VERTEX_SHADER, vertexShaderSource().c_str());
		shader.LoadFromString(GL_FRAGMENT_SHADER, fragmentShaderSource().c_str());

		shader.CreateAndLinkProgram();
		this->mesh = pmesh;
		bindexed = false;

	}

	virtual void Cleanup()
	{
		shader.Cleanup();
		vaoutl.Cleanup();
	}

	virtual void UpdateUniforms()
	{
		glEnable(GL_DEPTH_TEST);
		glUniformMatrix4fv(shader.GetUniformLocation("transform"), 1, GL_FALSE, glm::value_ptr(GetTransformationMatrix()));
	}

	virtual void Draw()
	{
		shader.Use();
		vaoutl.bindVAO();
		UpdateUniforms();

		if (!bindexed)
			glDrawArrays(GL_TRIANGLES, 0, kount);
		else
			glDrawElements(GL_TRIANGLES, kount, GL_UNSIGNED_SHORT, 0);

		vaoutl.unbindVAO();

		shader.Release();
	}

	void ReloadShader()
	{
		shader.Cleanup();
		shader.LoadFromString(GL_VERTEX_SHADER, vertexShaderSource().c_str());
		shader.LoadFromString(GL_FRAGMENT_SHADER, fragmentShaderSource().c_str());

		shader.CreateAndLinkProgram();
	}

	virtual string vertexShaderSource()
	{
		return R"(

		)";
	}

	virtual string fragmentShaderSource()
	{
		return R"(

		)";
	}

	mat4 GetTransformationMatrix()
	{
		return ProjectionMatrix * ViewMatrix * MM.getModelMatrix();
	}

public:
	ModelMatrixData MM;
	mat4 ViewMatrix{ 1 };
	mat4 ProjectionMatrix{ 1 };
	IGeometryMesh *mesh;

protected:
	ShaderUtil shader;
	VAOUtil vaoutl;
	GLuint kount;
	bool bindexed;

};
/////////////////////BaseGeometry///////////////////////////////////



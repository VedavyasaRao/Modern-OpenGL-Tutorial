#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include  "glm\gtx\transform.hpp"


using namespace std;
using namespace glm;

class ModelMatrixData
{
public:

	void augument(const ModelMatrixData& data)
	{
		Translateby += data.Translateby;
		Scaleby += data.Scaleby;
		agument(Pitch, data.Pitch);
		agument(Yaw , data.Yaw);
		agument(Roll, data.Roll);
	}

	void Reset()
	{
		Roll = 0;
		Pitch = 0;
		Yaw = 0;
		Scaleby = vec3(0);
		Translateby = vec3(0);
	}

	mat4 getModelMatrix()
	{
		auto M = mat4(1);
		M = translate(M, Translateby);
		M = rotate(M, radians((float)(Yaw)), vec3(0.0f, 1.0f, 0.0f));
		M = rotate(M, radians((float)(Pitch)), vec3(1.0f, 0.0f, 0.0f));
		M = rotate(M, radians((float)(Roll)), vec3(0.0f, 0.0f, 1.0f));
		M = scale(M, Scaleby);
		return M;
	}

protected:
	void agument(short& angle, short delta)
	{
		angle += delta;
		if (angle < 0)
			angle += 360;
		angle %= 360;
	}

public:
	short Roll = 0;
	short Pitch = 0;
	short Yaw = 0;
	vec3 Scaleby = vec3(1);
	vec3 Translateby = vec3(0);
};

class ViewMatrixData
{
public:
	void setViewMatrix(const vec3& position, const vec3& target, const vec3& up)
	{
		this->Position = position;
		this->Target = target;
		this->Up = up;
		V = lookAt(position, target, up);
	}

	mat4 getViewMatrix()
	{
		return V;
	}

public:
	vec3 Position;
	vec3 Target;
	vec3 Up;

protected:
	mat4 V{ 1 };
};

class PerspectiveProjectionMatrixData
{
public:
	void setAspectRatio(const float& aspectratio)
	{
		this->AspectRatio = aspectratio;
		P = perspective(radians((float)FOV), aspectratio, NearPlane, FarPlane);
	}

	void setFOV(int dir)
	{
		FOV += 2 * (std::signbit(dir*1.0) ? -1 : 1);
		setFOV();
	}

	void setFOV(float fovin)
	{
		FOV = (short)fovin;
		setFOV();
	}

	void setProjectionMatrix(float nearplane, float farplane)
	{
		this->NearPlane = nearplane;
		this->FarPlane = farplane;
		P = perspective(radians((float)FOV), AspectRatio, NearPlane, FarPlane);
	}

	mat4 getProjectionMatrix()
	{
		return  P;
	}

protected:
	void setFOV()
	{
		if (FOV < 10)
			FOV = 10;
		if (FOV > 135)
			FOV = 135;
		P = perspective(radians((float)FOV), AspectRatio, NearPlane, FarPlane);
	}

public:
	float AspectRatio = 1.7f;
	short FOV = 45;
	float NearPlane = 1.0;
	float FarPlane = 100.0;

protected:
	mat4 P{ 1 };
};

class OrthographicProjectionMatrixData
{
public:
	void setProjectionMatrix(vec2 xminmax, vec2 yminmax, vec2 zminmax)
	{
		this->XMinMax = xminmax;
		this->YMinMax = yminmax;
		this->ZMinMax = zminmax;
		//data.UpdateProjectionMatrix(glm::ortho(-aspectratio, aspectratio, -1.0f, 1.0f, 1.0f, 100.0f));
		P = glm::ortho(XMinMax.r, XMinMax.g, YMinMax.r, YMinMax.g, ZMinMax.r, ZMinMax.g);
	}

	mat4 getProjectionMatrix()
	{
		return  P;
	}

public:
	//orthographic
	vec2 XMinMax{ -1.0f, 1.0f };
	vec2 YMinMax{ -1.0f, 1.0f };
	vec2 ZMinMax{ 1.0f, 100.0f };

protected:
	mat4 P{ 1 };
};

#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
//#include "glm/ext/matrix_projection.hpp"

using namespace std;
using namespace glm;

class CameraData
{
public:

	void operator=(const CameraData& data)
	{
		roll = data.roll;
		pitch = data.pitch;
		yaw = data.yaw;
		scaleby = data.scaleby;
		translateby = data.translateby;
		V = data.V;
		P = data.P;
	}

	void augumentpitch(short delta)
	{
		agument(pitch, delta);
	}

	void augumentyaw(short delta)
	{
		agument(yaw, delta);
	}

	void augumentroll(short delta)
	{
		agument(roll, delta);
	}

	void updateModelMatrix()
	{
		M = mat4(1);
		M = translate(M, translateby);
		M = rotate(M, radians((float)(yaw)), vec3(0.0f, 1.0f, 0.0f));
		M = rotate(M, radians((float)(pitch)), vec3(1.0f, 0.0f, 0.0f));
		M = rotate(M, radians((float)(roll)), vec3(0.0f, 0.0f, 1.0f));
		M = scale(M, scaleby);
	}

	void UpdateProjectionMatrix(mat4 p)
	{
		P = p;
	}

	void UpdateViewMatrix(mat4 v)
	{
		V = v;
	}

	mat4 GetTransfermation()
	{
		updateModelMatrix();
		return P*V*M;
	}

	void updatePitch(short p)
	{
		pitch = p;
	}

	void updateRoll(short r)
	{
		roll = r;
	}

	void updateYaw(short y)
	{
		yaw = y;
	}

	void updateScale(const vec3& scaleby)
	{
		this->scaleby = scaleby;
	}

	void updateTranslate(const vec3& translateby)
	{
		this->translateby = translateby;
	}

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

protected:
	void agument(short& angle, short delta)
	{
		angle += delta;
		if (angle < 0)
			angle += 360;
		angle %= 360;
	}

protected:
	short roll = 0;
	short pitch = 0;
	short yaw = 0;
	vec3 scaleby = vec3(1);
	vec3 translateby = vec3(0);
	mat4 M = mat4(1);
	mat4 V = mat4(1);
	mat4 P = mat4(1);
	friend class BaseCameraInputHandler;
};

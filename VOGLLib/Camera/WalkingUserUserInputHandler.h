#pragma once
#include "PerspectiveProjectionCameraInputHandler.h"

/////////////////////WalkingUserUserInputHandler///////////////////////////////////
class WalkingUserUserInputHandler :public PerspectiveProjectionCameraInputHandler
{
public:
	WalkingUserUserInputHandler(HWND hwnd) :PerspectiveProjectionCameraInputHandler(hwnd) {}

	bool OnKey(int key)
	{
		if (PerspectiveProjectionCameraInputHandler::OnKey(key))
			return true;

		switch (key)
		{
		case 'W':
		case 'w':
			moveBy(1.0f * deltatime * _moveSpeed);
			break;

		case 'S':
		case 's':
			moveBy(-1.0f * deltatime * _moveSpeed);
			break;

		case 'A':
		case 'a':
			rotateBy(1.0f * deltatime * _rotationSpeed);
			break;

		case 'D':
		case 'd':
			rotateBy(-1.0f * deltatime * _rotationSpeed);
			break;

		default:
			return false;
		}

		return true;
	}

	virtual void UpdateViewMatrix()
	{
		data.UpdateViewMatrix(lookAt(_position, _viewPoint, _upVector));
	}

	void UpdateDeltaTime(float deltatime)
	{
		this->deltatime = deltatime;
		++frameCount;
		elapsedSeconds += deltatime;
	}

protected:
	void moveBy(float distance)
	{
		glm::vec3 vOffset = getNormalizedViewVector();
		vOffset *= distance;
		_position += vOffset;
		_viewPoint += vOffset;
	}

	void rotateBy(float angleInDegrees)
	{
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleInDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 rotatedViewVector = rotationMatrix * glm::vec4(getNormalizedViewVector(), 0.0f);
		_viewPoint = _position + glm::vec3(rotatedViewVector);
	}

	glm::vec3 getNormalizedViewVector() const
	{
		return  glm::normalize(_viewPoint - _position);
	}

protected:
	glm::vec3 _position = vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 _viewPoint = vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 _upVector = vec3(0.0f, 1.0f, 0.0f);
	float _moveSpeed = 10.0f;
	float _rotationSpeed = 135.0f; 
	float deltatime = 0.017f;
	int frameCount = 0;
	double elapsedSeconds=0.0;
};
/////////////////////WalkingUserUserInputHandler///////////////////////////////////


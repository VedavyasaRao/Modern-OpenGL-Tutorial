#pragma once
#include "WalkingUserUserInputHandler.h"

/////////////////////FlyingUserUserInputHandler///////////////////////////////////
class FlyingUserUserInputHandler :public WalkingUserUserInputHandler
{
public:
	FlyingUserUserInputHandler(HWND hwnd) :WalkingUserUserInputHandler(hwnd) {}
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
			strafeBy(1.0f * deltatime * _moveSpeed);
			break;

		case 'D':
		case 'd':
			strafeBy(-1.0f * deltatime * _moveSpeed);
			break;

		default:
			return false;
		}
		return true;
	}

	bool OnMouseMove(int button, int x, int y)
	{
		if (PerspectiveProjectionCameraInputHandler::OnMouseMove(button, x, y))
			return true;

		if (deltaX != 0 || deltaY != 0)
		{
			if (deltaX != 0)
			{
				rotateBy(-deltaX * _mouseSensitivity);
			}

			if (deltaY != 0)
			{
				rotateUpDown(-deltaY  * _mouseSensitivity);
			}
			
			return true;
		}
		return false;
	}

	wstring GetDisplayString()
	{
		static wstring wsfps = L"No Data";
		wostringstream outs;

		outs.precision(2);	
		if (elapsedSeconds > 0.50)
		{
			double fps = (double)frameCount / elapsedSeconds;
			double msPerFrame = 1000.0 / fps;
			outs << fixed
				<< "FPS: " << fps << endl
				<< "Frame Time: " << msPerFrame << " (ms)";

			frameCount = 0;
			elapsedSeconds = 0.0;
			wsfps = outs.str();
		}

		frameCount++;
		return wsfps;
	}

protected:
	void strafeBy(float distance)
	{
		glm::vec3 strafeVector = glm::normalize(glm::cross(getNormalizedViewVector(), _upVector));
		strafeVector = glm::normalize(strafeVector);
		strafeVector *= distance;

		_position += strafeVector;
		_viewPoint += strafeVector;
	}

	void rotateUpDown(float angleInDegrees)
	{
		const glm::vec3 viewVector = getNormalizedViewVector();
		const glm::vec3 viewVectorNoY = glm::normalize(glm::vec3(viewVector.x, 0.0f, viewVector.z));

		float currentAngleDegrees = glm::degrees(acos(glm::dot(viewVectorNoY, viewVector)));
		if (viewVector.y < 0.0f) {
			currentAngleDegrees = -currentAngleDegrees;
		}

		float newAngleDegrees = currentAngleDegrees + angleInDegrees;
		if (newAngleDegrees > -85.0f && newAngleDegrees < 85.0f)
		{
			glm::vec3 rotationAxis = glm::cross(getNormalizedViewVector(), _upVector);
			rotationAxis = glm::normalize(rotationAxis);

			glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleInDegrees), rotationAxis);
			glm::vec4 rotatedViewVector = rotationMatrix * glm::vec4(getNormalizedViewVector(), 0.0f);

			_viewPoint = _position + glm::vec3(rotatedViewVector);
		}
	}
protected:
	float _mouseSensitivity = 0.15f;
};
/////////////////////FlyingUserUserInputHandler///////////////////////////////////
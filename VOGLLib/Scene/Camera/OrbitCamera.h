#pragma once
#include "PerspectiveProjectionCamera.h"

/////////////////////OrbitCamera///////////////////////////////////
class OrbitCamera :public PerspectiveProjectionCamera
{
public:
	OrbitCamera(HWND hwnd) :PerspectiveProjectionCamera(hwnd) {}

	bool OnKey(int key)
	{
		if (PerspectiveProjectionCamera::OnKey(key))
			return true;

		switch (key)
		{
		case VK_LEFT:
			zoom(zoomfactor);
			break;

		case VK_RIGHT:
			zoom(-zoomfactor);
			break;

		default:
			return false;
		}
		return true;
	}

	bool OnMouseMove(int button, int x, int y)
	{
		if (PerspectiveProjectionCamera::OnMouseMove(button, x, y))
			return true;

		if (mouseRightDown)
		{
			rotateAzimuth(deltaX * 0.01f);
			rotatePolar(deltaY * 0.01f);
			return true;
		}

		if (mouseMiddleDown)
		{
			moveHorizontal(-deltaX * 0.05f);
			moveVertical(deltaY * 0.05f);
			return true;
		}

		return false;
	}


	void OrbitCamera::rotateAzimuth(const float radians)
	{
		azimuthAngle_ += radians;

		// Keep azimuth angle within range <0..2PI) - it's not necessary, just to have it nicely output
		const auto fullCircle = 2.0f*glm::pi<float>();
		azimuthAngle_ = fmodf(azimuthAngle_, fullCircle);
		if (azimuthAngle_ < 0.0f) {
			azimuthAngle_ = fullCircle + azimuthAngle_;
		}
	}

	void OrbitCamera::rotatePolar(const float radians)
	{
		polarAngle_ += radians;

		// Check if the angle hasn't exceeded quarter of a circle to prevent flip, add a bit of epsilon like 0.001 radians
		const auto polarCap = glm::pi<float>() / 2.0f - 0.001f;
		if (polarAngle_ > polarCap) {
			polarAngle_ = polarCap;
		}

		if (polarAngle_ < -polarCap) {
			polarAngle_ = -polarCap;
		}
	}

	void OrbitCamera::zoom(const float distance)
	{
		radius_ += distance;
		if (radius_ < _minRadius) {
			radius_ = _minRadius;
		}
	}

	void OrbitCamera::moveHorizontal(const float distance)
	{
		const auto position = getEye();
		const glm::vec3 viewVector = getNormalizedViewVector();
		const glm::vec3 strafeVector = glm::normalize(glm::cross(viewVector, upVector_));
		center_ += strafeVector * distance;
	}

	void OrbitCamera::moveVertical(const float distance)
	{
		center_ += upVector_ * distance;
	}

	void OrbitCamera::updateViewMatrix() 
	{
		auto eye = getEye();
		VM.setViewMatrix(eye, eye + getNormalizedViewVector(), upVector_);
	}

	glm::vec3 OrbitCamera::getEye() const
	{
		// Calculate sines / cosines of angles
		const auto sineAzimuth = sin(azimuthAngle_);
		const auto cosineAzimuth = cos(azimuthAngle_);
		const auto sinePolar = sin(polarAngle_);
		const auto cosinePolar = cos(polarAngle_);

		// Calculate eye position out of them
		const auto x = center_.x + radius_ * cosinePolar * cosineAzimuth;
		const auto y = center_.y + radius_ * sinePolar;
		const auto z = center_.z + radius_ * cosinePolar * sineAzimuth;

		return glm::vec3(x, y, z);
	}

	glm::vec3 OrbitCamera::getViewPoint() const
	{
		return center_;
	}

	glm::vec3 OrbitCamera::getUpVector() const
	{
		return upVector_;
	}

	glm::vec3 OrbitCamera::getNormalizedViewVector() const
	{
		return glm::normalize(getViewPoint() - getEye());
	}


protected:
	glm::vec3 center_ = { 0.0f, 0.0f, 0.0f };
	glm::vec3 upVector_ = { 0.0, 1.0f, 0.0f };
	float radius_ = 4.0f;
	float _minRadius = 1.0f; 
	float azimuthAngle_= glm::pi<float>() * 0.5f;
	float polarAngle_ = 0.0f;
	float zoomfactor = 0.5f;
	float mouse_sensevity = 0.5f;

};
/////////////////////OrbitCamera///////////////////////////////////


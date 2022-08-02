#pragma once
#include "PerspectiveProjectionCameraInputHandler.h"

/////////////////////OrbitCameraInputHandler///////////////////////////////////
class OrbitCameraInputHandler :public PerspectiveProjectionCameraInputHandler
{
public:
	OrbitCameraInputHandler(HWND hwnd) :PerspectiveProjectionCameraInputHandler(hwnd) {}

	bool OnKey(int key)
	{
		if (PerspectiveProjectionCameraInputHandler::OnKey(key))
			return true;

		switch (key)
		{
		case VK_LEFT:
			zoom(0.5);
			break;

		case VK_RIGHT:
			zoom(-0.5);
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

		if (mouseRightDown)
		{
			zoom(-(deltaY) * 0.2f);
			return true;
		}

		return false;
	}

	virtual wstring GetDisplayString()
	{
		WCHAR buf[300];
		swprintf_s(buf, 300, L"%s %0.2f %d", BaseCameraInputHandler::GetDisplayString().c_str(), radius_, fov);
		return buf;
	}

	virtual void UpdateViewMatrix()
	{
		// https://stackoverflow.com/questions/19740463/lookat-function-im-going-crazy
		const auto position = getEye();

		const auto f = getNormalizedViewVector();
		auto u = glm::normalize(upVector_);
		const auto s = glm::normalize(cross(f, u));
		u = cross(s, f);

		glm::mat4 resultViewMatrix(1.0f);
		resultViewMatrix[0][0] = s.x;
		resultViewMatrix[1][0] = s.y;
		resultViewMatrix[2][0] = s.z;
		resultViewMatrix[0][1] = u.x;
		resultViewMatrix[1][1] = u.y;
		resultViewMatrix[2][1] = u.z;
		resultViewMatrix[0][2] = -f.x;
		resultViewMatrix[1][2] = -f.y;
		resultViewMatrix[2][2] = -f.z;
		resultViewMatrix[3][0] = -glm::dot(s, position);
		resultViewMatrix[3][1] = -glm::dot(u, position);
		resultViewMatrix[3][2] = glm::dot(f, position);
		data.UpdateViewMatrix(resultViewMatrix);
	}

protected:
	void rotatePolar(const float radians)
	{
		polarAngle_ += radians;

		// Keep polar angle within range <0..2PI) - it's not necessary, just to have it nicely output
		const auto fullCircle = 2.0f*glm::pi<float>();
		polarAngle_ = fmodf(polarAngle_, fullCircle);
		if (polarAngle_ < 0.0f) {
			polarAngle_ = fullCircle + polarAngle_;
		}
	}

	void rotateAzimuth(const float radians)
	{
		azimuthAngle_ += radians;

		// Check if the angle hasn't exceeded quarter of a circle to prevent flip, add a bit of epsilon like 0.001 radians
		const auto azimuthCap = glm::pi<float>() / 2.0f - 0.001f;
		if (azimuthAngle_ > azimuthCap) {
			azimuthAngle_ = azimuthCap;
		}

		if (azimuthAngle_ < -azimuthCap) {
			azimuthAngle_ = -azimuthCap;
		}
	}

	void zoom(const float by)
	{
		radius_ -= by;
		if (radius_ < _minRadius) {
			radius_ = _minRadius;
		}
	}

	void moveHorizontal(const float by)
	{
		const auto position = getEye();
		glm::vec3 viewVector = glm::normalize(center_ - position);
		glm::vec3 strafeVector = glm::normalize(glm::cross(viewVector, upVector_));
		center_ += strafeVector * by;
	}

	void moveVertical(const float by)
	{
		center_ += upVector_ * by;
	}

	glm::vec3 getEye() const
	{
		// Calculate sines / cosines of angles
		const auto sinePolar = sin(polarAngle_);
		const auto cosinePolar = cos(polarAngle_);
		const auto sineAzimuth = sin(azimuthAngle_);
		const auto cosineAzimuth = cos(azimuthAngle_);

		// Calculate eye position out of them
		const auto t = radius_ * cosineAzimuth;
		const auto y = center_.y + radius_ * sineAzimuth;
		const auto x = center_.x + t * cosinePolar;
		const auto z = center_.z + t * sinePolar;

		return glm::vec3(x, y, z);
	}

	glm::vec3 getViewPoint() const
	{
		return center_;
	}

	glm::vec3 getUpVector() const
	{
		return upVector_;
	}

	glm::vec3 getNormalizedViewVector() const
	{
		return glm::normalize(getViewPoint() - getEye());
	}

	float getPolarAngle() const
	{
		return polarAngle_;
	}

	float getAzimuthAngle() const
	{
		return azimuthAngle_;
	}

	float getRadius() const
	{
		return radius_;
	}

protected:
	glm::vec3 center_ = glm::vec3(0.0f, 0.0f, 0.0f); // Center of the orbit camera sphere (the point upon which the camera looks)
	glm::vec3 upVector_= glm::vec3(0.0, 1.0f, 0.0f); // Up vector of the camera
	float radius_= 15.0f; // Radius of the orbit camera sphere
	float _minRadius = 3.0f; // Minimal radius of the orbit camera sphere (cannot fall below this value)
	float polarAngle_= glm::pi<float>() * 0.5f; // Polar angle on the orbit camera sphere
	float azimuthAngle_= 0.0f; // Azimuth angle on the orbit camera sphere
};
/////////////////////OrbitCameraInputHandler///////////////////////////////////


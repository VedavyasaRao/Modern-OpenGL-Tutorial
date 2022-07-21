#pragma once
#include "PerspectiveProjectionCameraInputHandler.h"

/////////////////////FPSCameraInputHandler///////////////////////////////////
class FPSCameraInputHandler :public PerspectiveProjectionCameraInputHandler
{
public:
	FPSCameraInputHandler(HWND hwnd) :PerspectiveProjectionCameraInputHandler(hwnd) {}

	bool OnKey(int key)
	{
		if (PerspectiveProjectionCameraInputHandler::OnKey(key))
			return true;

		switch (key)
		{
		case 'W':
		case 'w':
			moveforward(delta);
			break;

		case 'S':
		case 's':
			movebackward(delta);
			break;

		case 'D':
		case 'd':
			moveleft(delta);
			break;

		case 'A':
		case 'a':
			moveright(delta);
			break;

		case 'K':
		case 'k':
			moveup(delta);
			break;

		case 'L':
		case 'l':
			movedown(delta);
			break;

		default:
			return false;
		}
		return true;
	}

	bool OnMouseMove(int button, int x, int y)
	{
		auto w = windowrect.right - windowrect.left;
		auto h = windowrect.bottom - windowrect.top;

		if (x <= 10 || x >= w - 10 || y <= 10 || y >= h - 10)
		{
			SetCursorPos (windowrect.left + w / 2, windowrect.top + h / 2);
			mouseX = 0;
			return false;
		}

		if (mouseX == 0)
		{
			mouseX = x;
			mouseY = y;
		}

		if (PerspectiveProjectionCameraInputHandler::OnMouseMove(button, x, y))
			return true;

		if (!mouseLeftDown && !mouseRightDown)
		{
			int offsetx = mouseX - x;
			int offsety = y - mouseY;
			mouseX = x;
			mouseY = y;
			ProcessMouseMovement((float)offsetx, (float)offsety);
			return true;
		}
		return false;
	}

	virtual void UpdateViewMatrix()
	{
		data.UpdateViewMatrix(lookAt(Position, Position + Front, Up));
	}

	wstring GetDisplayString()
	{
		static double previousSeconds = GetTickCount() / 1000;
		static int frameCount = 0;
		static wstring wsfps = L"No Data";
		double elapsedSeconds;
		double currentSeconds = GetTickCount() / 1000; // returns number of seconds since GLFW started, as double float
		wostringstream outs;

		elapsedSeconds = currentSeconds - previousSeconds;
		outs.precision(2);	// decimal places

							// Limit text updates to 4 times per second
		if (elapsedSeconds > 0.25)
		{
			previousSeconds = currentSeconds;
			double fps = (double)frameCount / elapsedSeconds;
			double msPerFrame = 1000.0 / fps;

			// The C++ way of setting the window title
			outs << fixed
				<< "FPS: " << fps << endl
				<< "Frame Time: " << msPerFrame << " (ms)";
			// Reset for next average.
			frameCount = 0;
			wsfps = outs.str();
		}

		frameCount++;
		return wsfps;
	}

	void updatedelta(float delta)
	{
		this->delta = delta;
	}

protected:
	void moveforward(float delta)
	{
		Position += Front  * delta * MovementSpeed;
	}

	void movebackward(float delta)
	{
		Position -= Front * delta  * MovementSpeed;
	}

	void moveup(float delta)
	{
		Position -= Up  * delta  * MovementSpeed;
	}

	void movedown(float delta)
	{
		Position += Up  * delta * MovementSpeed;
	}

	void moveright(float delta)
	{
		Position += Right  * delta  * MovementSpeed;
	}

	void moveleft(float delta)
	{
		Position -= Right  * delta  * MovementSpeed;
	}

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	void updateCameraVectors()
	{
		// calculate the new Front vector
		vec3 front;
		front.x = cos(radians(Yaw)) * cos(radians(Pitch));
		front.y = sin(radians(Pitch));
		front.z = sin(radians(Yaw)) * cos(radians(Pitch));
		Front = normalize(front);
		// also re-calculate the Right and Up vector
		Right = normalize(cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = normalize(cross(Right, Front));

	}

protected:
	float delta;
	vec3 Position = vec3(0.0f, 0.0f, 2.0f);
	vec3 Front = vec3(0.0f, 0.0f, -1.0f);
	vec3 Up = vec3(0.0f, 1.0f, 0.0f);
	vec3 Right;
	vec3 WorldUp = vec3(0.0f, 1.0f, 0.0f);
	// euler Angles
	float Yaw = -90.0f;
	float Pitch = 0.0f;
	// camera options
	float MovementSpeed = 2.5f;
	float MouseSensitivity = 0.1f;

};
/////////////////////FPSCameraInputHandler///////////////////////////////////


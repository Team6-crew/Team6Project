/******************************************************************************
Class:Camera
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:FPS-Style camera. Uses the mouse and keyboard from the Window
class to get movement values!

-_-_-_-_-_-_-_,------,
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""

*//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Window.h"
#include "Matrix4.h"
#include "Vector3.h"

class Camera {
public:
	Camera(void) {
		position = nclgl::Maths::Vector3(0.0f, 0.0f, 0.0f);
		yaw = 0.0f;
		pitch = 0.0f;
	};

	Camera(float pitch, float yaw, nclgl::Maths::Vector3 position) {
		this->pitch = pitch;
		this->yaw = yaw;
		this->position = position;
	}

	~Camera(void) {};

	void HandleMouse(float dt);
	void HandleKeyboard(float dt);

	//Builds a view matrix for the current camera variables, suitable for sending straight
	//to a vertex shader (i.e it's already an 'inverse camera matrix').
	nclgl::Maths::Matrix4 BuildViewMatrix();

	//Gets position in world space
	inline const nclgl::Maths::Vector3& GetPosition() const { return position; }
	//Sets position in world space
	void	SetPosition(const nclgl::Maths::Vector3& val) { position = val; }

	//Gets yaw, in degrees
	float	GetYaw()   const { return yaw; }
	//Sets yaw, in degrees
	void	SetYaw(float y) { yaw = y; }

	//Gets pitch, in degrees
	float	GetPitch() const { return pitch; }
	//Sets pitch, in degrees
	void	SetPitch(float p) { pitch = p; }

	nclgl::Maths::Vector3 GetFacingDirection(); 
	
protected:
	float				  yaw;
	float				  pitch;
	nclgl::Maths::Vector3 position;
};
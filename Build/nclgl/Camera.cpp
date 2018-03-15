#include "Camera.h"

using namespace nclgl::Maths;
/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::HandleMouse(float dt)
{

	//Update the mouse by how much
	//if (OGLWindow::GetMouse()->ButtonDown(MOUSE_LEFT))
	//{
	//	pitch -= (OGLWindow::GetMouse()->GetRelativePosition().y);
	//	yaw -= (OGLWindow::GetMouse()->GetRelativePosition().x);
	//}

	//float wheel_speed = OGLWindow::GetMouse()->GetWheelMovement() * 0.5f;
	//Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Matrix4::Rotation(pitch, Vector3(1, 0, 0));
	//position += rotation * Vector3(0, 0, -1) * wheel_speed;


	//Bounds check the pitch, to be between straight up and straight down ;)
	//pitch = min(pitch, 90.0f);
	//pitch = max(pitch, -90.0f);

	//if (yaw < 0.0f) {
	//	yaw += 360.0f;
	//}
	//if (yaw > 360.0f) {
	//	yaw -= 360.0f;
	//}
}

void Camera::HandleKeyboard(float dt)
{
	std::cout << "Handling input!!!" << std::endl;
	float speed = 3.5f * dt;	//3.5m per second


								//Bounds check the pitch, to be between straight up and straight down ;)
	if (yaw <0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}


	//if (OGLWindow::GetKeyboard()->KeyDown(KEYBOARD_W)) {
	//	position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * speed;
	//}
	//if (OGLWindow::GetKeyboard()->KeyDown(KEYBOARD_S)) {
	//	position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * speed;
	//}

	//if (OGLWindow::GetKeyboard()->KeyDown(KEYBOARD_A)) {
	//	position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * speed;
	//}
	//if (OGLWindow::GetKeyboard()->KeyDown(KEYBOARD_D)) {
	//	position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * speed;
	//}

	//if (OGLWindow::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
	//	position.y += speed;
	//}
	//if (OGLWindow::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
	//	position.y -= speed;
	//}
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix() {
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return	Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
		Matrix4::Translation(-position);
};

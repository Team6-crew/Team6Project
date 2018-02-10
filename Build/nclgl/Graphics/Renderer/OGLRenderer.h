#pragma once

#include "RenderBase.h"

#include <windows.h> // Windows specific functions and constants

class Window;

#include "../../Matrix4.h"

class OGLRenderer : public RenderBase
{
public:
	OGLRenderer(Window& window);
	~OGLRenderer();

	void SwapBuffers() override;
	void RenderObject(RenderNode* obj) override;

	void Resize(int x, int y) override;

	Matrix4	GetViewMatrix() override;
	Matrix4 GetProjMatrix() override;
	void 	SetViewMatrix(Matrix4& mat) override;
	void	SetProjMatrix(Matrix4& mat) override;
protected:
	// Windows specific handles
	HDC		deviceContext;	
	HGLRC	renderContext;	

	// Initialisation Functions

	// Setup the front and back buffers
	bool CreateBuffers();
	bool SetupOpenGLContext();


protected:
	Matrix4 viewMatrix;
	Matrix4 projMatrix;

private:
	// OpenGL version
	int major = 3;
	int minor = 2;
};


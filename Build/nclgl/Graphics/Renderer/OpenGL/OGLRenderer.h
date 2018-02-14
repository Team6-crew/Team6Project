#pragma once

#include "../RenderBase.h"

#include <windows.h> // Windows specific functions and constants

class Window;

#include "../../../Matrix4.h"


class OGLRenderer : public RenderBase
{
public:
	OGLRenderer(Window& window);
	~OGLRenderer();

	void SwapBuffers() override;
	void RenderObject(RenderNode* obj) override;

	void Resize(int x, int y) override;

	nclgl::Maths::Matrix4	GetViewMatrix() override;
	nclgl::Maths::Matrix4 GetProjMatrix() override;
	void 	SetViewMatrix(nclgl::Maths::Matrix4& mat) override;
	void	SetProjMatrix(nclgl::Maths::Matrix4& mat) override;

	void	SetViewPort(int width, int height) override;
	void	Clear(Renderer::Clear clearType) override;
	void	SetClearColour(nclgl::Maths::Vector3& vec) override;

	void	BindScreenFramebuffer() override;


	void	SetScreenCulling(Renderer::Culling type) override;
	void	SetDefaultSettings() override;
protected:
	// Windows specific handles
	HDC		deviceContext;	
	HGLRC	renderContext;	

	// Initialisation Functions

	// Setup the front and back buffers
	bool CreateBuffers();
	bool SetupOpenGLContext();


protected:
	nclgl::Maths::Matrix4 viewMatrix;
	nclgl::Maths::Matrix4 projMatrix;

private:
	// OpenGL version
	int major = 3;
	int minor = 2;
};


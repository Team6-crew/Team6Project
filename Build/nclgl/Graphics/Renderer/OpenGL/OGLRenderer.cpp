#ifdef WIN_OGL
#include "OGLRenderer.h"

#include <cassert>

#include <nclgl\Window.h>

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL\glew.h>
#include <GL\wglew.h>


using namespace Renderer;
using namespace nclgl::Maths;

OGLRenderer::OGLRenderer(Window& window)
{
	initSuccess = false;

	HWND windowHandle = window.GetHandle();

	if (!(deviceContext = GetDC(windowHandle))) 
	{
		return;
	}

	if (!CreateBuffers())
	{
		return;
	}

	if (!SetupOpenGLContext())
	{
		return;
	}
	
	glewExperimental = GL_TRUE;	//This forces GLEW to give us function pointers for everything (gets around GLEW using 'old fashioned' methods
								//for determining whether a OGL context supports a particular function or not
	if (glewInit() != GLEW_OK) {	//Try to initialise GLEW
		return;
	}

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);			//When we clear the screen, we want it to be dark grey

	window.SetRenderer(this);					//Tell our window about the new renderer! (Which will in turn resize the renderer window to fit...)
	initSuccess = true;
}


OGLRenderer::~OGLRenderer()
{
}

// We call the windows OS SwapBuffers on win32.
void OGLRenderer::SwapBuffers()
{
	::SwapBuffers(deviceContext);
}
//void OGLRenderer::RenderObject(RenderNode* obj)
//{
//	wglDeleteContext(renderContext);
//}

void OGLRenderer::Resize(int x, int y)
{
	width = max(x, 1);
	height = max(y, 1);
	glViewport(0, 0, width, height);
}

void 	OGLRenderer::SetViewMatrix(Matrix4& mat) 
{
	viewMatrix = mat;
}
void	OGLRenderer::SetProjMatrix(Matrix4& mat)
{
	projMatrix = mat;
}

bool OGLRenderer::CreateBuffers()
{
	//A pixel format descriptor is a struct that tells the Windows OS what type of front / back buffers we want to create etc
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   //It must be double buffered, it must support OGL(!), and it must allow us to draw to it...
	pfd.iPixelType = PFD_TYPE_RGBA;	//We want our front / back buffer to have 4 channels!
	pfd.cColorBits = 32;				//4 channels of 8 bits each!
	pfd.cDepthBits = 24;				//24 bit depth buffer
	pfd.cStencilBits = 8;				//plus an 8 bit stencil buffer
	pfd.iLayerType = PFD_MAIN_PLANE;

	GLuint		PixelFormat;
	if (!(PixelFormat = ChoosePixelFormat(deviceContext, &pfd))) {	// Did Windows Find A Matching Pixel Format for our PFD?
		return false;
	}

	if (!SetPixelFormat(deviceContext, PixelFormat, &pfd)) {		// Are We Able To Set The Pixel Format?
		return false;
	}

	return true;
}


bool OGLRenderer::SetupOpenGLContext()
{

	HGLRC		tempContext;		//We need a temporary OpenGL context to check for OpenGL 3.2 compatibility...stupid!!!
	if (!(tempContext = wglCreateContext(deviceContext))) {	// Are We Able To get the temporary context?
		std::cout << "OGLRenderer::OGLRenderer(): Cannot create a temporary context!" << std::endl;
		wglDeleteContext(tempContext);
		return false;
	}

	if (!wglMakeCurrent(deviceContext, tempContext)) {	// Try To Activate The Rendering Context
		std::cout << "OGLRenderer::OGLRenderer(): Cannot set temporary context!" << std::endl;
		wglDeleteContext(tempContext);
		return false;
}
	//We do support OGL 3! Let's set it up...
	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,	
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
		,WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,		//We want everything OpenGL 3.2 provides...
		0					//That's enough attributes...
	};

	//Everywhere else in the Renderers, we use function pointers provided by GLEW...but we can't initialise GLEW yet! So we have to use the 'Wiggle' API
	//to get a pointer to the function that will create our OpenGL 3.2 context...
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	renderContext = wglCreateContextAttribsARB(deviceContext, 0, attribs);

	wglDeleteContext(tempContext);	//We don't need the temporary context any more!

	// Check for the context, and try to make it the current rendering context
	if (!renderContext || !wglMakeCurrent(deviceContext, renderContext)) {
		wglDeleteContext(renderContext);
		return false;
	}
}

Matrix4	OGLRenderer::GetViewMatrix()
{
	return viewMatrix;
}

Matrix4 OGLRenderer::GetProjMatrix()
{
	return projMatrix;
}

void	OGLRenderer::SetViewPort(int width, int height)
{
	glViewport(0, 0, width, height);
}

void	OGLRenderer::Clear(Renderer::Clear clearType)
{
	switch (clearType)
	{
	case DEPTH:
		glClear(GL_DEPTH_BUFFER_BIT);
		break;
	case COLOUR:
		glClear(GL_COLOR_BUFFER_BIT);
		break;
	case COLOUR_DEPTH:
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		break;
	}
}

void	OGLRenderer::SetClearColour(Vector3& col)
{
	glClearColor(col.x, col.y, col.z, 1.0f);
}

void	OGLRenderer::BindScreenFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void	OGLRenderer::SetScreenCulling(Culling type)
{
	switch (type)
	{
	case FRONT:
		glCullFace(GL_FRONT);
		break;
	case BACK:
		glCullFace(GL_BACK);
		break;
	}
}
void	OGLRenderer::SetDefaultSettings()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
#endif
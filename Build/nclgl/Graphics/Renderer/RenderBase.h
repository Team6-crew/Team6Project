#pragma once

class RenderNode;
class Matrix4;
class Vector3;

namespace Renderer
{
	enum Clear { COLOUR, DEPTH, COLOUR_DEPTH };
	enum Culling { FRONT, BACK};
};

class RenderBase
{
public:
	RenderBase();
	virtual ~RenderBase();

	virtual void SwapBuffers() = 0;
	virtual void RenderObject(RenderNode* obj) = 0;

	// Resizes the rendering area to [x, y].
	virtual void Resize(int x, int y) = 0;

	bool WasInitialised() { return initSuccess;  }


	inline int GetWidth()  { return width; }
	inline int GetHeight() { return height; }

	virtual Matrix4	GetViewMatrix() = 0;
	virtual Matrix4 GetProjMatrix() = 0;
	virtual void 	SetViewMatrix(Matrix4& mat) = 0;
	virtual void    SetProjMatrix(Matrix4& mat) = 0;

	virtual void	SetViewPort(int width, int height) = 0;
	virtual void	Clear(Renderer::Clear clearType) = 0;
	virtual void	SetClearColour(Vector3& colour) = 0;

	virtual void	BindScreenFramebuffer() = 0;


	virtual void	SetScreenCulling(Renderer::Culling type) = 0;
	virtual void	SetDefaultSettings() = 0;

protected:
	bool initSuccess;

	// Width and height of the renderable area
	int width;
	int height;
};
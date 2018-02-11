#pragma once

class RenderNode;
class Matrix4;

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

protected:
	bool initSuccess;

	// Width and height of the renderable area
	int width;
	int height;
};
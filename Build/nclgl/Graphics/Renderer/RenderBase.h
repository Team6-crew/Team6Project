#pragma once

class RenderNode;

namespace nclgl
{
	namespace Maths
	{
		class Matrix4;
		class Vector3;
	}
}


class ShaderBase;
class RenderNodeBase;
class TextureBase;
class FrameBufferBase;

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
	//virtual void RenderObject(RenderNode* obj) = 0;

	// Resizes the rendering area to [x, y].
	virtual void Resize(int x, int y) = 0;

	bool WasInitialised() { return initSuccess;  }

	virtual void	PrepareToRender() = 0;
	virtual void	PostRender() = 0;


	virtual void	RegisterShader(ShaderBase* s) = 0;
	virtual void	RegisterTexture(TextureBase* s) = 0;
	virtual void	RegisterNode(RenderNodeBase* s) = 0;
	virtual void	RegisterBuffer(FrameBufferBase* s) = 0;

	inline int GetWidth()  { return width; }
	inline int GetHeight() { return height; }

	virtual nclgl::Maths::Matrix4	GetViewMatrix() = 0;
	virtual nclgl::Maths::Matrix4   GetProjMatrix() = 0;
	virtual void 					SetViewMatrix(const nclgl::Maths::Matrix4& mat) = 0;
	virtual void					SetProjMatrix(const nclgl::Maths::Matrix4& mat) = 0;

	virtual void	SetViewPort(int width, int height) = 0;
	virtual void	Clear(Renderer::Clear clearType) = 0;
	virtual void	SetClearColour(nclgl::Maths::Vector3& colour) = 0;

	virtual void	BindScreenFramebuffer() = 0;

	virtual void	SetScreenCulling(Renderer::Culling type) = 0;
	virtual void	SetDefaultSettings() = 0;

protected:
	bool initSuccess;

	// Width and height of the renderable area
	int width;
	int height;
};
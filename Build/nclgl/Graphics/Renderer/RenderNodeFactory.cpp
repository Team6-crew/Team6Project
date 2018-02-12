#include "RenderNodeFactory.h"

#include "OpenGL\OGLRenderNode.h"

RenderNodeFactory::RenderNodeFactory()
{
}


RenderNodeFactory::~RenderNodeFactory()
{
}

RenderNodeBase* RenderNodeFactory::MakeRenderNode(MeshBase* mesh, Vector4 colour)
{
	#ifdef WIN_OGL
		return new OGLRenderNode(mesh, colour);
	#endif
}
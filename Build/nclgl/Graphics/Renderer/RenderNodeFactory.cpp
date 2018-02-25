#include "RenderNodeFactory.h"

#include "OpenGL\OGLRenderNode.h"
#include "PS4\PS4RenderNode.h"

RenderNodeFactory::RenderNodeFactory()
{
}


RenderNodeFactory::~RenderNodeFactory()
{
}

RenderNodeBase* RenderNodeFactory::MakeRenderNode(MeshBase* mesh, nclgl::Maths::Vector4 colour)
{
	#ifdef WIN_OGL
		return new OGLRenderNode(mesh, colour);
	#elif PSTATION4
		return new PS4RenderNode(mesh, colour);
	#endif
}
#include "RenderNodeFactory.h"

#include "OpenGL\OGLRenderNode.h"
#include "PS4\PS4RenderNode.h"

RenderNodeFactory::RenderNodeFactory()
{
}


RenderNodeFactory::~RenderNodeFactory()
{
}


RenderNodeBase* RenderNodeFactory::MakeRenderNode(MeshBase* mesh, OBJMeshBase* objmesh, nclgl::Maths::Vector4 colour)
{
	#ifdef WIN_OGL
		return new OGLRenderNode(mesh, objmesh, colour);
	#elif PSTATION4
		return new PS4RenderNode(mesh, objmesh, colour);
	#endif
}



#include "MeshFactory.h"
#include "Graphics\Renderer\OpenGL\OGLMesh.h"
#include "Graphics\Renderer\PS4\PS4Mesh.h"
#include "OGLOBJMesh.h"
#include "PS4OBJMesh.h"

OBJMeshBase * MeshFactory::MakeOBJMesh(std::string file)
{
#ifdef WIN_OGL
	return new OGLOBJMesh(file);
#elif PSTATION4
	return new PS4OBJMesh(file);
#endif
}

MeshBase * MeshFactory::MakeMesh()
{
#ifdef WIN_OGL
	return new OGLMesh();
#elif PSTATION4
	return new PS4Mesh();
#endif
}

MeshBase * MeshFactory::GenerateQuad()
{
#ifdef WIN_OGL
	return OGLMesh::GenerateQuad();
#elif PSTATION4
	//needs implemeting
	return PS4Mesh::GenerateQuad();
#endif
}

MeshFactory::MeshFactory()
{
}


MeshFactory::~MeshFactory()
{
}

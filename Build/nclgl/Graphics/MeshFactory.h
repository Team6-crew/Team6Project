#pragma once

#include <nclgl/TSingleton.h>

#include <vector>

class MeshBase;
class TextureBase;
class OBJMeshBase;

#ifdef WIN_OGL
#define MESHDIR		"../../Data/Meshes/"
#elif PSTATION4
#define MESHDIR		"/app0/Data/Meshes/"
#endif // DEBUG

class MeshFactory : public TSingleton<MeshFactory>
{
	friend class TSingleton<MeshFactory>;
public:
	OBJMeshBase * MakeOBJMesh(std::string file);
	MeshBase * MakeMesh();
	MeshBase * GenerateQuad();
private:
	MeshFactory();
	~MeshFactory();
};


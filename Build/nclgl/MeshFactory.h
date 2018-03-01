#pragma once

#include <nclgl/TSingleton.h>

#include <vector>

class MeshBase;
class TextureBase;
class OBJMeshBase;

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


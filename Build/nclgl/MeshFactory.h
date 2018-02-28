#pragma once

#include <nclgl/TSingleton.h>

#include <vector>

class MeshBase;
class TextureBase;

class MeshFactory : public TSingleton<MeshFactory>
{
	friend class TSingleton<MeshFactory>;
public:
	MeshBase * MakeMesh();
	MeshBase * GenerateQuad();
private:
	MeshFactory();
	~MeshFactory();
};


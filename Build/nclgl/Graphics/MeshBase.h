#pragma once

class TextureBase;

class MeshBase
{
public:
	MeshBase();
	virtual ~MeshBase();

	virtual void Draw() = 0;
	virtual void SetTexture(TextureBase* texture) = 0;
};


#pragma once
class MeshBase
{
public:
	MeshBase();
	virtual ~MeshBase();

	virtual void Draw() = 0;
	virtual void SetTexture(int temp) = 0;
};


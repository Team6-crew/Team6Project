#pragma once
#include <vector>
class TextureBase;

class MeshBase
{
public:
	MeshBase();
	virtual ~MeshBase();

	virtual void Draw() = 0;
	virtual void SetTexture(TextureBase* texture) = 0;
	virtual TextureBase* GetTexture(int index) { return textures[index]; }
	virtual void ReplaceTexture(TextureBase* texture, int pos) = 0;
protected:
	std::vector<TextureBase *> textures;
};


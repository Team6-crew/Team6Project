//A texture manager used by the resource manager for loading, storing and retriving textures

#pragma once

#include "Graphics\Renderer\TextureFactory.h"
#include "Graphics\TextureBase.h"
#include <unordered_map>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	TextureBase* get(std::string &name);
	TextureBase* makeTexture(std::string& name, Texture::Type &type, int& a, int& b);
private:
	std::unordered_map<std::string, TextureBase*> textureMap;
};


#include "ResourceManager.h"



ResourceManager::ResourceManager():textureManager()
{
}


ResourceManager::~ResourceManager()
{
}

TextureBase * ResourceManager::get(std::string name)
{
	return textureManager.get(name);
}

TextureBase * ResourceManager::MakeTexture(std::string name, Texture::Type type, int a, int b)
{
	return textureManager.makeTexture(name, type, a, b);
}



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

#include "ResourceManager.h"



ResourceManager::ResourceManager():textureManager()
{
}


ResourceManager::~ResourceManager()
{
}

TextureBase * ResourceManager::getTexture(std::string name)
{
	return textureManager.get(name);
}

TextureBase * ResourceManager::MakeTexture(std::string name, Texture::Type type, int a, int b)
{
	return textureManager.makeTexture(name, type, a, b);
}

ShaderBase * ResourceManager::getShader(std::string vertex, std::string fragment, std::string geometry)
{
	return shaderManager.get(vertex,fragment,geometry);
}



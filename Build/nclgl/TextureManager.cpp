#include "TextureManager.h"



TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
	for (auto it : textureMap) {
		delete it.second;
	}
}

TextureBase * TextureManager::get(std::string &name)
{
	if (textureMap.count(name) == 0) {
		TextureBase* newTexture = TextureFactory::Instance()->MakeTexture(name);
		textureMap.insert(std::make_pair(name, newTexture));
	}
	return textureMap.at(name);
}

TextureBase * TextureManager::makeTexture(std::string & name, Texture::Type & type, int & a, int & b)
{
	TextureBase* newTexture = TextureFactory::Instance()->MakeTexture(type, a, b);
	textureMap.insert(std::make_pair(name, newTexture));
	return textureMap.at(name);
}

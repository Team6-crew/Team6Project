//A Class used for managing resources(Textures, Audio Files, Meshes)

#pragma once
#include <nclgl\TSingleton.h>
#include "TextureManager.h"

class ResourceManager : public TSingleton<ResourceManager> {
public:
	ResourceManager();
	~ResourceManager();

	TextureBase* get(std::string name);
	TextureBase* MakeTexture(std::string name, Texture::Type type, int a, int b);




private:
	TextureManager textureManager;
};


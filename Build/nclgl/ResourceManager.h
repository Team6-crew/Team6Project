//A Class used for managing resources(Textures, Audio Files, Meshes, Shaders)
//Certain resources can be created with a make method however some can only be loaded from file
//In the case of resources that are loaded from file a get method is to be called with the file to load from, this then creates the resource if it 
//	doesnt exist and returns a pointer to the newly created resource, or alternativly if the resource exists a pointer to the pre existing resource 
//	is returned
#pragma once
#include <nclgl\TSingleton.h>
#include "TextureManager.h"
#include "ShaderManager.h"

class ResourceManager : public TSingleton<ResourceManager> {
public:
	ResourceManager();
	~ResourceManager();

	TextureBase* getTexture(std::string name);
	TextureBase* MakeTexture(std::string name, Texture::Type type, int a, int b);


	ShaderBase* getShader(std::string vertex, std::string fragment, std::string geometry);

private:
	TextureManager textureManager;
	ShaderManager shaderManager;
};


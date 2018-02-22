//a shader manager for storing and loading shader files for use by the resource manager
#pragma once
#include "Graphics\Renderer\ShaderFactory.h"
#include "Graphics\ShaderBase.h"
#include <unordered_map>

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	//get a shader from file, if already exists the shader is returend
	ShaderBase* get(std::string vertex, std::string fragment, std::string geometry = "");
	
private:
	std::unordered_map<std::string, ShaderBase*> shaderMap;
};


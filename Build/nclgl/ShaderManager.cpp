#include "ShaderManager.h"



ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}

ShaderBase * ShaderManager::get(std::string vertex, std::string fragment, std::string geometry)
{
	std::string name = vertex + fragment + geometry;
	if (shaderMap.count(name) == 0) {
		ShaderBase* newShader = ShaderFactory::Instance()->MakeShader(vertex, fragment, geometry);
		shaderMap.insert(std::make_pair(name, newShader));
	}
	return shaderMap.at(name);
}

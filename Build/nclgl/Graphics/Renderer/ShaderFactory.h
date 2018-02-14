#pragma once

#include <nclgl\TSingleton.h>
#include <string>

class ShaderBase;


class ShaderFactory : public TSingleton<ShaderFactory>
{
	friend class TSingleton<ShaderFactory>;
public:
	ShaderBase* MakeShader(std::string vertex, std::string fragment, std::string geometry = "");

private:
	ShaderFactory();
	~ShaderFactory();
};


#include "ShaderFactory.h"

#include <nclgl\Graphics\Renderer\OpenGL\OGLShader.h>


ShaderFactory::ShaderFactory()
{
}


ShaderFactory::~ShaderFactory()
{
}

ShaderBase* ShaderFactory::MakeShader(std::string vertex, std::string fragment, std::string geometry)
{
#ifdef WIN_OGL
	return new OGLShader(vertex, fragment, geometry);
#endif

}

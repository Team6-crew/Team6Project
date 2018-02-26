#include "ShaderFactory.h"

#include <nclgl\Graphics\Renderer\OpenGL\OGLShader.h>
#include <nclgl\Graphics\Renderer\PS4\PS4Shader.h>

ShaderFactory::ShaderFactory()
{
}


ShaderFactory::~ShaderFactory()
{
}

ShaderBase* ShaderFactory::MakeShader(std::string vertex, std::string fragment, std::string geometry)
{
#ifdef WIN_OGL
	return new OGLShader(vertex.append(".glsl"), fragment.append(".glsl"), geometry.append(".glsl"));
#elif PSTATION4
	return new PS4Shader(vertex.append(".pssl"), fragment.append(".pssl"), geometry.append(".pssl"));
#endif

}

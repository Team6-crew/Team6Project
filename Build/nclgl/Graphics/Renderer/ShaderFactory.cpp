#include "ShaderFactory.h"

#include <nclgl\Graphics\Renderer\OpenGL\OGLShader.h>
#include <nclgl\Graphics\Renderer\PS4\PS4Shader.h>

#ifdef WIN_OGL
#define SHADERDIR	"../../Data/Shaders/"
#elif PSTATION4
#define SHADERDIR	"/app0/Data/Shaders/"
#endif // DEBUG



ShaderFactory::ShaderFactory()
{
}


ShaderFactory::~ShaderFactory()
{
}

ShaderBase* ShaderFactory::MakeShader(std::string vertex, std::string fragment, std::string geometry)
{
#ifdef WIN_OGL
	return new OGLShader(SHADERDIR + vertex.append(".glsl"), 
						 SHADERDIR + fragment.append(".glsl"), 
						 SHADERDIR + geometry.append(".glsl"));
#elif PSTATION4
	return new PS4Shader(SHADERDIR + vertex.append(".sb"), SHADERDIR + fragment.append(".sb"));
#endif

}

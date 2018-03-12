#include "TextureFactory.h"

#include "OpenGL/OGLTexture.h"
#include "OpenGL/OGLTextureArray.h"

#include "PS4\PS4Texture.h"
#include "PS4\PS4TextureArray.h"

#include "RenderFactory.h"
#include <nclgl\Graphics\Renderer\PS4\PS4Renderer.h>


using namespace TextureTypeNamespace;

TextureFactory::TextureFactory()
{
}


TextureFactory::~TextureFactory()
{
}


TextureBase* TextureFactory::MakeTexture(std::string filepath)
{
	#ifdef WIN_OGL
	return new OGLTexture(TEXTUREDIR + filepath.append(".tga"));
	#elif  PSTATION4
		return new PS4Texture(TEXTUREDIR + filepath.append(".gnf"));
	#endif 

	
}

TextureBase* TextureFactory::MakeTexture(TextureTypeNamespace::Type type, int a, int b)
{
	switch (type)
	{
	case DEPTH:
	case COLOUR:
		#ifdef WIN_OGL
			return new OGLTexture(type, a, b);
		#elif PSTATION4
		return new PS4Texture(type, a, b, static_cast<PS4Renderer*>(RenderFactory::Instance()->GetRenderer())->AllocateMemory(GARLIC, a * b * 4));
		#endif 
		break;
	case DEPTH_ARRAY:
	#ifdef WIN_OGL
			return new OGLTextureArray(type, a, b);
	#elif PSTATION4
			return nullptr;
	#endif 
		break;

		
	}
}
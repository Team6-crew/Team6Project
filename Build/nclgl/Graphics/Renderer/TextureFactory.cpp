#include "TextureFactory.h"

#include "OpenGL/OGLTexture.h"
#include "OpenGL/OGLTextureArray.h"

#include "PS4\PS4Texture.h"
#include "PS4\PS4TextureArray.h"


using namespace Texture;

TextureFactory::TextureFactory()
{
}


TextureFactory::~TextureFactory()
{
}


TextureBase* TextureFactory::MakeTexture(std::string& filepath)
{
	#ifdef WIN_OGL
	return new OGLTexture(filepath.append(".tga"));
	#elif  PSTATION4
		//return new PS4Texture(filepath.append(".gnf"));
	#endif 

	
}

TextureBase* TextureFactory::MakeTexture(Texture::Type type, int a, int b)
{
	switch (type)
	{
	case DEPTH:
	case COLOUR:
		#ifdef WIN_OGL
			return new OGLTexture(type, a, b);
		#elif PSTATION4
			return new PS4Texture(type, a, b);
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
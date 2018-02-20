#include "TextureFactory.h"

#include "OpenGL/OGLTexture.h"
#include "OpenGL/OGLTextureArray.h"


using namespace Texture;

TextureFactory::TextureFactory()
{
}


TextureFactory::~TextureFactory()
{
}


TextureBase* TextureFactory::MakeTexture(const std::string& filepath)
{
	#ifdef WIN_OGL
		return new OGLTexture(filepath);
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
		#endif 
		break;
	case DEPTH_ARRAY:
#ifdef WIN_OGL
		return new OGLTextureArray(type, a, b);
#endif 
		break;

		
	}
}
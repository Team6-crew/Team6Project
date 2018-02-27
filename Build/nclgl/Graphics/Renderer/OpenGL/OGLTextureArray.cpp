#ifdef WIN_OGL
#include "OGLTextureArray.h"

using namespace Texture;

#include <iostream>
#include <cassert>

OGLTextureArray::OGLTextureArray(Type type, int dimensions, int numTextures)
{
	glGenTextures(1, &textureID);

	SetTextureFiltering();
	SetTextureWrapping();

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	if (type == DEPTH_ARRAY)
	{
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT32, dimensions, dimensions, numTextures);
	}
	else
	{
		std::cout << "Inappropriate texture type" << std::endl;
		assert(false);
	}
}


OGLTextureArray::~OGLTextureArray()
{
	glDeleteTextures(1, &textureID);
}

void OGLTextureArray::Bind(int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
}

void OGLTextureArray::SetTextureFiltering(bool nearest)
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void OGLTextureArray::SetTextureWrapping(bool repeating)
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

int OGLTextureArray::TempGetID()
{
	return textureID;
}

void OGLTextureArray::LoadTexture(const std::string& filepath)
{

}
#endif
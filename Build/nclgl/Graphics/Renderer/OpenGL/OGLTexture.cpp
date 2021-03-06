#include "OGLTexture.h"

#include <SOIL.h>
#include <iostream>
#include <cassert>

using namespace Texture;

DEFINE_HEAP(OGLTexture, "Graphics");
OGLTexture::OGLTexture(const std::string& filepath)
{
	LoadTexture(filepath);
	if (loadSuccess)
	{
		SetTextureFiltering();
		SetTextureWrapping();
	}
	//TODO: Get Width and Height?
}

OGLTexture::OGLTexture(Type type, int width, int height)
{
	this->width = width;
	this->height = height;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (type == COLOUR)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}
	else if (type == DEPTH)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	else
	{
		std::cout << "Inappropriate texture type" << std::endl;
		assert(false);
	}

	SetTextureFiltering();
	SetTextureWrapping();
}

OGLTexture::~OGLTexture()
{
	glDeleteTextures(1, &textureID);
}


void OGLTexture::Bind(int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}


void OGLTexture::LoadTexture(const std::string& filepath)
{
	textureID = SOIL_load_OGL_texture(filepath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (!textureID) {
		std::cout << filepath << " " << SOIL_last_result() << std::endl;
	}
	loadSuccess = bool(textureID);

}

void OGLTexture::SetTextureFiltering(bool nearest)
{
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (nearest)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}


}
void OGLTexture::SetTextureWrapping(bool repeating)
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (repeating)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

int OGLTexture::TempGetID()
{
	return textureID;
}

GLuint OGLTexture::GetID()
{
	return textureID;
}
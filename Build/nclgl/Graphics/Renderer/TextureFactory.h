#pragma once

#include <nclgl\TSingleton.h>
#include <string>

#include <nclgl/Graphics/TextureBase.h>

class TextureFactory : public TSingleton<TextureFactory>
{
	friend class TSingleton<TextureFactory>;
public:
	TextureBase* MakeTexture(const std::string& filepath);

	// Meaning of a and b depend on type of texture
	TextureBase* MakeTexture(Texture::Type type, int a, int b);
private:
	TextureFactory();
	~TextureFactory();
};
#pragma once

#include <nclgl\TSingleton.h>
#include <string>

#include <nclgl/Graphics/TextureBase.h>

enum Type { COLOUR, DEPTH, DEPTH_ARRAY };

class TextureFactory : public TSingleton<TextureFactory>
{
	friend class TSingleton<TextureFactory>;
public:
	TextureBase* MakeTexture(std::string filepath);

	// Meaning of a and b depend on type of texture
	TextureBase* MakeTexture(Type type, int a, int b);
private:
	TextureFactory();
	~TextureFactory();


};
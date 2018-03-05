#pragma once

#include <nclgl\TSingleton.h>
#include <string>
#include "../TextureBase.h"
#include <nclgl/Graphics/TextureBase.h>

#ifdef WIN_OGL
#define TEXTUREDIR  "../../Data/Textures/"
#elif PSTATION4
#define TEXTUREDIR  "/app0/Data/Shaders/"
#endif // DEBUG



class TextureFactory : public TSingleton<TextureFactory>
{
	friend class TSingleton<TextureFactory>;
public:
	// Meaning of a and b depend on type of texture
	TextureBase * MakeTexture(TextureTypeNamespace::Type type, int a, int b);

	TextureBase* MakeTexture(std::string filepath);


private:
	TextureFactory();
	~TextureFactory();


};
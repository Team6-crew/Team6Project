#ifdef PSTATION4

#pragma once


#include <string>

#include "PS4MemoryAware.h"
#include "../../TextureBase.h"

class PS4Texture : public PS4MemoryAware, public TextureBase
{
public:
	PS4Texture();
	PS4Texture(const std::string& filepath);
	PS4Texture(TextureTypeNamespace::Type type, int a, int b);
	~PS4Texture();

	void SetCurrentGFXContext(sce::Gnmx::GnmxGfxContext* GFXContext);
	void Bind(int textureUnit = 0) override;
	void SetTextureFiltering(bool nearest = false) override;
	void SetTextureWrapping(bool repeating = false) override;


	sce::Gnm::Texture apiTexture;
protected:
	

	bool LoadTexture(const std::string& filepath) override;

	sce::Gnmx::GnmxGfxContext*	currentGFXContext = nullptr;

	sce::Gnm::Sampler trilinearSampler;

	//bool repeating;
	//bool 
};

#endif
#ifdef PSTATION4
#include "PS4Texture.h"

#include <fstream>

#include <gnf.h>

using namespace TextureTypeNamespace;
using namespace sce;
PS4Texture::PS4Texture()
{
	trilinearSampler.init();
	trilinearSampler.setMipFilterMode(sce::Gnm::kMipFilterModeLinear);
}

PS4Texture::PS4Texture(const std::string& filepath)
{
	LoadTexture(filepath);
	if (loadSuccess)
	{
	SetTextureFiltering();
	SetTextureWrapping();
	}

	trilinearSampler.init();
	trilinearSampler.setMipFilterMode(sce::Gnm::kMipFilterModeLinear);
}

#include <iostream>
PS4Texture::PS4Texture(Type type, int a, int b)
{
	switch (type)
	{
	case COLOUR:
	//	apiTexture.initAs2d(a, b, 1, sce::Gnm::DataFormat::build(Gnm::RenderTargetFormat::kRenderTargetFormat8_8_8_8, Gnm::RenderTargetChannelType::kRenderTargetChannelTypeSrgb,  Gnm::RenderTargetChannelOrder::kRenderTargetChannelOrderStandard), sce::Gnm::TileMode::kTileModeDisplay_2dThin, sce::Gnm::NumFragments::kNumFragments1);
		apiTexture.initAs2d(a, b, 1, sce::Gnm::DataFormat::build(sce::Gnm::SurfaceFormat::kSurfaceFormat8_8_8_8, sce::Gnm::kTextureChannelTypeSrgb, sce::Gnm::kTextureChannelX, sce::Gnm::kTextureChannelY, sce::Gnm::kTextureChannelZ, sce::Gnm::kTextureChannelW), sce::Gnm::TileMode::kTileModeDisplay_2dThin, sce::Gnm::NumFragments::kNumFragments1);
	//	apiTexture.initAs2d(a, b, 1, sce::Gnm::DataFormat::build(sce::Gnm::BufferFormat::kBufferFormat8_8_8_8, sce::Gnm::BufferChannelType::kBufferChannelTypeFloat), sce::Gnm::TileMode::kTileModeDisplay_2dThin, sce::Gnm::NumFragments::kNumFragments1);
		apiTexture.setTextureType(sce::Gnm::kTextureType2d);
		
		break;

	case DEPTH:
		//sce::Gnm::DataFormat dformat;
		//dformat.build(sce::Gnm::ZFormat::kZFormat32Float);
		//dformat.build(sce::Gnm::SurfaceFormat::kSurfaceFormat32, sce::Gnm::kTextureChannelTypeFloat, sce::Gnm::kTextureChannelX, sce::Gnm::kTextureChannelX, sce::Gnm::kTextureChannelX, sce::Gnm::kTextureChannelX);
		apiTexture.initAs2d(a, b, 1, sce::Gnm::DataFormat::build(sce::Gnm::SurfaceFormat::kSurfaceFormat32, sce::Gnm::kTextureChannelTypeFloat, sce::Gnm::kTextureChannelX, sce::Gnm::kTextureChannelX, sce::Gnm::kTextureChannelX, sce::Gnm::kTextureChannelX), sce::Gnm::TileMode::kTileModeDepth_2dThin_64, sce::Gnm::NumFragments::kNumFragments1);
		apiTexture.setTextureType(sce::Gnm::kTextureType2d);
		//apiTexture.setResourceMemoryType(sce::Gnm::kResourceMemoryTypeRO);
		//apiTexture.setChannelType(sce::Gnm::kTextureChannelTypeFloat);
		//apiTexture.setChannelOrder(sce::Gnm::kTextureChannelX, sce::Gnm::kTextureChannelX, sce::Gnm::kTextureChannelX, sce::Gnm::kTextureChannelX);
		break;
	}
}

PS4Texture::~PS4Texture()
{
}


bool PS4Texture::LoadTexture(const std::string& filepath)
{
	std::ifstream file(filepath, std::ios::binary);

	if (!file) {
		return false;
	}


	// Extract the Gnf header - contentsSize and magicNumber
	sce::Gnf::Header header;
	file.read((char*)&header, sizeof(header));

	// Is it a gnf file?
	if (header.m_magicNumber != sce::Gnf::kMagic) {
		return false;
	}

	// Load texture data
	char* rawContents = new char[header.m_contentsSize];
	file.read((char*)rawContents, header.m_contentsSize);

	// Allocate GPU and bus memory
	sce::Gnf::Contents* contentsDesc = (sce::Gnf::Contents*)rawContents;
	sce::Gnm::SizeAlign dataParams = getTexturePixelsSize(contentsDesc, 0);

	void *pixelsAddr = garlicAllocator.allocate(dataParams);
	sce::Gnm::registerResource(nullptr, ownerHandle, pixelsAddr, dataParams.m_size, filepath.c_str(), sce::Gnm::kResourceTypeTextureBaseAddress, 0);

	// Move to the start of the texture data (assume one texture per file)
	file.seekg(getTexturePixelsByteOffset(contentsDesc, 0), ios::cur);
	file.read((char*)pixelsAddr, dataParams.m_size);

	apiTexture = *patchTextures(contentsDesc, 0, 1, &pixelsAddr);
	apiTexture.setResourceMemoryType(sce::Gnm::kResourceMemoryTypeRO);
	width =	apiTexture.getWidth();
	height = apiTexture.getHeight();

	file.close();
	delete[] rawContents;

	return true;
}

void PS4Texture::SetCurrentGFXContext(sce::Gnmx::GnmxGfxContext* GFXContext)
{

	currentGFXContext = GFXContext;
	currentGFXContext->setSamplers(sce::Gnm::kShaderStagePs, 0, 1, &trilinearSampler);

}

void PS4Texture::Bind(int textureUnit)
{
	if (currentGFXContext)
	{
		currentGFXContext->setTextures(sce::Gnm::kShaderStagePs, textureUnit, 1, &apiTexture);
	}

}



void PS4Texture::SetTextureFiltering(bool nearest)
{

}
void PS4Texture::SetTextureWrapping(bool repeating)
{

}

#endif
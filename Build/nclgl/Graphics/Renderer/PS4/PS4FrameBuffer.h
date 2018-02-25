#pragma once

#include <vector>
#include <gnm.h>
#include <gnmx\fetchshaderhelper.h>
#include "PS4MemoryAware.h"
#include <..\samples\sample_code\graphics\api_gnm\toolkit\toolkit.h>
#include <nclgl\common.h>

#include "../../FrameBufferBase.h"
using namespace sce;
using namespace Gnmx;
using namespace Gnmx::Toolkit;

class PS4Texture;
class TextureBase;

class PS4FrameBuffer : public PS4MemoryAware, public FrameBufferBase
{
public:
	void InitialiseMemoryAllocators();
	~PS4FrameBuffer();

	PS4FrameBuffer(TextureBase* colourTex, TextureBase* depthTex);
	void SetMem(void * colourMemory, void * depthMemory);
	PS4FrameBuffer(std::vector<TextureBase*> colourTex, PS4Texture* depthTex);
	PS4FrameBuffer(TextureBase* depthTex, bool colour);

	uint GetWidth() override;
	uint GetHeight() override;

	void Activate() override;


	inline void SetGraphicsContext(sce::Gnmx::GnmxGfxContext* cmdList) { currentGFXContext = cmdList; }

	void ClearBuffer();

	uint bufferID;


	sce::Gnm::RenderTarget		colourTarget;
	sce::Gnm::DepthRenderTarget depthTarget;
protected:

	uint width;
	uint height;


	PS4Texture* colourTexture;
	PS4Texture* depthTexture;

	// Current graphics context
	sce::Gnmx::GnmxGfxContext*	   currentGFXContext = nullptr;
};


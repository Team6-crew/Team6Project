#ifdef PSTATION4
#pragma once

#include <gnmx\context.h>
#include <gnm.h>

#include "PS4MemoryAware.h"
#include <graphics\api_gnm\toolkit\geommath\geommath.h>
#include "../RenderBase.h"
#include <vector>

class PS4FrameBuffer;
class PS4Frame;
class PS4RenderNode;



namespace nclgl
{
	namespace Maths
	{
		class Matrix4;
		class Vector3;
	}
}

//class Vector3;
class PS4ScreenBuffer;

class PS4Shader;
class PS4Texture;

class PS4Renderer : public PS4MemoryAware, public RenderBase
{

public:
	PS4Renderer();
	~PS4Renderer();

	// Make override when integrate
	void	SwapBuffers() override;

	void	SetViewPort(int width, int height) override;
	void	Clear(Renderer::Clear clearType) override;
	void	SetClearColour(nclgl::Maths::Vector3& colour) override;

	// PREPARE TO RENDER PREPARE TO RENDER PREPARE TO RENDER PREPARE TO RENDER
	void	PrepareToRender() override;
	void	PostRender() override;
	void	BindScreenFramebuffer() override;

	void Resize(int x, int y) override;

	void	SetScreenCulling(Renderer::Culling type) override;
	void	SetDefaultSettings() override;

	// Getters/Setters
	nclgl::Maths::Matrix4	GetViewMatrix() override;
	nclgl::Maths::Matrix4   GetProjMatrix() override;
	void 					SetViewMatrix(const nclgl::Maths::Matrix4& mat) override;
	void					SetProjMatrix(const nclgl::Maths::Matrix4& mat) override;

	void					RegisterShader(ShaderBase* s)  override;
	void					RegisterTexture(TextureBase* s)override;
	void					RegisterNode(RenderNodeBase* s)override;
	void					RegisterBuffer(FrameBufferBase* s) override;

	void*					AllocateMemory(MemoryLocation location, uint sizeBytes);
protected:// Setup functions 
	void			 InitialiseMemoryAllocators();
	void			 InitialiseVideoSystem();
	void			 InitialiseGCMRendering();
	PS4ScreenBuffer* GenerateScreenBuffer(uint width, uint height, bool colour = true, bool depth = true, bool stencil = false);
	PS4FrameBuffer*	 GenerateScreenFrameBuffer(uint width, uint height, bool colour = true, bool depth = true, bool stencil = false);


protected: // Clean up functions
	void			DestroyMemoryAllocators();
	void			DestroyVideoSystem();
	void			DestroyGCMRendering();
protected: // States
	sce::Gnm::PrimitiveSetup	  primitiveSetup;
	sce::Gnm::BlendControl		  blendControlSetup;
	sce::Gnm::DepthStencilControl depthControlSetup;

	sce::Gnmx::GnmxGfxContext*	  currentGFXContext;


protected: // Swap buffer
	void SwapScreenBuffer();
	void SwapCommandBuffer();
	//SCREEN BUFFER VARIABLES
	const int			numBuffers;			//How many screen buffers should we have
	int					currentScreenBuffer;
	int					prevScreenBuffer;
	//PS4ScreenBuffer**	screenBuffers;	//Pointer to our screen buffers
	PS4FrameBuffer**	FrameBuffers;	//Pointer to our screen buffers

										//Per frame pointers...
	PS4ScreenBuffer*		currentPS4Buffer;  //Pointer to whichever buffer we're currently using...
	PS4FrameBuffer*			currentPS4FrameBuffer;

	// COMMAND BUFFER VARIABLES (i.e. bits I don't understand very well)
	const int   maxCMDBuffers;
	PS4Frame*	currentFrame;
	PS4Frame*	frames;
	int currentGPUBuffer = 0;
	int framesSubmitted;
	void	SetRenderFrameBuffer(PS4FrameBuffer * buffer, bool clearColour, bool clearDepth, bool clearStencil);

	//VIDEO SYSTEM VARIABLES
	int videoHandle;		//Handle to video system

							// Memory Buses
	sce::Gnmx::Toolkit::StackAllocator	stackAllocators[MEMORYMAX];
	const int garlicMemoryBytes;
	const int onionMemoryBytes;

	sce::Vectormath::Scalar::Aos::Matrix4 projMatrix;
	sce::Vectormath::Scalar::Aos::Matrix4 viewMatrix;

	std::vector<PS4Texture*> textures;
	std::vector<PS4RenderNode*> nodes;
	std::vector<PS4Shader*> shaders;
	std::vector<PS4FrameBuffer*> buffers;
};

#endif
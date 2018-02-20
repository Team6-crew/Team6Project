#pragma once

#include <nclgl\TSingleton.h>
#include <nclgl\Camera.h>
#include <nclgl/Graphics/Renderer/RenderNodeBase.h>
#include <nclgl\Definitions.h>
#include <vector>
#include <nclgl\Graphics\RenderConstants.h>

#include <GL/glew.h>

#include <vector>

class FrameBufferBase;
class ShaderBase;
class RenderBase;
class RenderNodeBase;
class MeshBase;
class TextureBase;

typedef std::pair<RenderNodeBase*, float> TransparentPair;


class GraphicsPipeline : public TSingleton<GraphicsPipeline>
{
	friend class TSingleton<GraphicsPipeline>;

public:
	//Set all defaults
	// - Called each time a scene is swapped in/reset, so should reset camera position etc
	void InitializeDefaults();

	//Add/Remove Render objects to renderable object lists
	void AddRenderNode(RenderNodeBase* node);
	void RemoveRenderNode(RenderNodeBase* node);

	//Called by main game loop
	// - Naming convention from oglrenderer
	void UpdateScene(float dt);
	void RenderScene();

	//Utils
	inline Camera* GetCamera() { return camera; }

	Camera* CreateNewCamera();

protected:
	GraphicsPipeline();
	virtual ~GraphicsPipeline();

	void Resize(int x, int y); //Called by window when it is resized

	void LoadShaders();
	void UpdateAssets(int width, int height);
	void BuildAndSortRenderLists();
	void RecursiveAddToRenderLists(RenderNodeBase* node);
	void RenderAllObjects(bool isShadowPass, std::function<void(RenderNodeBase*)> perObjectFunc = NULL);
	void BuildShadowTransforms(); //Builds the shadow projView matrices
	void AdjustViewport(int i, int j);

protected:

	RenderBase* renderer = nullptr;


	nclgl::Maths::Matrix4 projViewMatrix;

	FrameBufferBase*	renderFBO;
	uint				screenTexWidth, screenTexHeight;
	FrameBufferBase*	screenFBO;
	TextureBase*		screenTexColor;
	TextureBase*		screenTexDepth;

	//Shaders
	ShaderBase* shaderPresentToWindow;
	ShaderBase* shaderShadow;
	ShaderBase* shaderForwardLighting;

	ShaderBase* shaderTrail;

	//Render Params
	nclgl::Maths::Vector3	ambientColor;
	float					gammaCorrection;	//Monitor Default: 1.0 / 2.2 (Where 2.2 here is the gamma of the monitor which we need to invert before doing lighting calculations)		
	nclgl::Maths::Vector3	lightDirection;
	nclgl::Maths::Vector3	backgroundColor;
	float					specularFactor;
	uint					numSuperSamples;


	//Shadowmaps
	float								sceneBoundingRadius; ///Approx based on scene contents
	FrameBufferBase*					shadowFBO;
	TextureBase*						shadowTex;
	nclgl::Maths::Matrix4				shadowProj[SHADOWMAP_NUM];
	nclgl::Maths::Matrix4				shadowViewMtx;
	nclgl::Maths::Matrix4				shadowProjView[SHADOWMAP_NUM];
	float								normalizedFarPlanes[SHADOWMAP_NUM - 1];

	//Common
	MeshBase* fullscreenQuad;
	Camera* camera;
	/*Camera* camera1;
	Camera* camera2;*/
	bool isVsyncEnabled;
	std::vector<RenderNodeBase*> allNodes;

	std::vector<RenderNodeBase*> renderlistOpaque;
	std::vector<TransparentPair> renderlistTransparent;	//Also stores cameraDist in the second argument for sorting purposes
	std::vector<Camera*> cameras;
	std::vector<nclgl::Maths::Matrix4> viewMatrices;
	std::vector<nclgl::Maths::Matrix4> projViewMatrices;

	MeshBase* trailQuad;

	int world_paint[GROUND_TEXTURE_SIZE][GROUND_TEXTURE_SIZE];

	TextureBase* gr_tex;
	FrameBufferBase* TrailBuffer;

	float paint_perc;

	//Minimap
	MeshBase* minimap;
	nclgl::Maths::Matrix4 tempProj;
	nclgl::Maths::Matrix4 tempView;
};
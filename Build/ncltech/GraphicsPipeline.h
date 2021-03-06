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
class GameObject;
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
	void RenderMenu();
	void RenderScene(float dt);
	void LoadingScreen(float frame);
	void FillPaint(float dt);
	void StartCounter();
	void BuffHUD(int playerIndex);
	void SplatProjectile(float pos_x, float pos_z, float rad, nclgl::Maths::Vector4 trailColor);
	//Utils
	inline Camera* GetCamera() { return camera; }
	Camera* GetCameras(int cam) { return cameras[cam]; }
	void pushPaintableObject(GameObject * Paintable) { paintableObjects.push_back(Paintable); };
	Camera* CreateNewCamera();
	Camera* RepairCamera(int cmr);
	void ChangeScene();

	void clearGraphicsPipeline();
protected:
	GraphicsPipeline();
	virtual ~GraphicsPipeline();

	void Resize(int x, int y); //Called by window when it is resized
	float flickerPie;
	bool toggleFlicker;
	void LoadShaders();
	void UpdateAssets(int width, int height);
	void BuildAndSortRenderLists();
	void RecursiveAddToRenderLists(RenderNodeBase* node);
	void RenderAllObjects(bool isShadowPass, std::function<void(RenderNodeBase*)> perObjectFunc = NULL);
	void BuildShadowTransforms(); //Builds the shadow projView matrices
	void AdjustViewport(int i, int j);


protected:

	RenderBase* renderer = nullptr;

	float totalTime;
	nclgl::Maths::Matrix4 projViewMatrix;

	FrameBufferBase*	renderFBO;
	uint				screenTexWidth, screenTexHeight;
	FrameBufferBase*	screenFBO;

	//Shaders
	ShaderBase* shaderPresentToWindow;
	ShaderBase* shaderShadow;
	ShaderBase* shaderForwardLighting;
	ShaderBase* shaderLoading;
	ShaderBase* shaderTrail;
	ShaderBase* shaderCircle;
	ShaderBase* shaderPaint;
	ShaderBase* shaderSplat;
	ShaderBase* shaderPaintable;
	ShaderBase* shaderMap;
	ShaderBase* shaderCounter;
	ShaderBase* shaderBuff;
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
	nclgl::Maths::Matrix4				shadowProj[SHADOWMAP_NUM];
	nclgl::Maths::Matrix4				shadowViewMtx;
	nclgl::Maths::Matrix4				shadowProjView[SHADOWMAP_NUM];
	float								normalizedFarPlanes[SHADOWMAP_NUM - 1];

	//Common
	MeshBase* fullscreenQuad;

	MeshBase* paintQuad;
	Camera* camera;
	/*Camera* camera1;
	Camera* camera2;*/
	bool isVsyncEnabled;
	std::vector<RenderNodeBase*> allNodes;
	std::vector<GameObject*> paintableObjects;
	std::vector<RenderNodeBase*> renderlistOpaque;
	std::vector<TransparentPair> renderlistTransparent;	//Also stores cameraDist in the second argument for sorting purposes
	std::vector<Camera*> cameras;
	std::vector<nclgl::Maths::Matrix4> viewMatrices;
	std::vector<nclgl::Maths::Matrix4> projViewMatrices;

	MeshBase* trailQuad;

	int world_paint[GROUND_TEXTURE_SIZE][GROUND_TEXTURE_SIZE];

	FrameBufferBase* TrailBuffer;
	FrameBufferBase* CircleBuffer;
	FrameBufferBase* LoadingBuffer;
	FrameBufferBase* PaintBuffer;
	FrameBufferBase* BuffBuffer;
	TextureBase* temp_tex;
	TextureBase* loading_tex;
	TextureBase* splat_tex;
	TextureBase * tex_1;
	TextureBase * tex_2;
	TextureBase * tex_3;
	TextureBase* buff_paint_tex;
	TextureBase* buff_stun_tex;
	TextureBase* buff_speed_tex;

	float paint_perc;

	//Minimap
	MeshBase* minimap;
	MeshBase* piemap;
	nclgl::Maths::Matrix4 tempProj;
	nclgl::Maths::Matrix4 tempView;

	RenderNodeBase * ground;

	bool sound321played;

	
};
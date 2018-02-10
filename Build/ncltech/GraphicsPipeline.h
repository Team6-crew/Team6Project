#pragma once


#include <nclgl\TSingleton.h>
#include <nclgl\Camera.h>
#include <nclgl\RenderNode.h>
#include <nclgl\Graphics\RenderConstants.h>

typedef std::pair<RenderNode*, float> TransparentPair;


class Shader;
class RenderBase;

class GraphicsPipeline : public TSingleton<GraphicsPipeline>
{
	friend class TSingleton<GraphicsPipeline>;

public:
	//Set all defaults
	// - Called each time a scene is swapped in/reset, so should reset camera position etc
	void InitializeDefaults();

	//Add/Remove Render objects to renderable object lists
	void AddRenderNode(RenderNode* node);
	void RemoveRenderNode(RenderNode* node);




	//Called by main game loop
	// - Naming convention from oglrenderer
	void UpdateScene(float dt);
	void RenderScene();



	//Utils
	inline Camera* GetCamera() { return camera; }
	//inline bool GetVsyncEnabled() const { return isVsyncEnabled; }
	//inline void SetVsyncEnabled(bool enabled) { wglSwapIntervalEXT((isVsyncEnabled = enabled) ? 1 : 0); }

	inline Matrix4& GetProjMtx() { return renderer->GetProjMatrix(); }
	inline Matrix4& GetViewMtx() { return renderer->GetViewMatrix(); }

	inline Matrix4& GetShadowViewMtx() { return shadowViewMtx; }
	inline Matrix4* GetShadowProjMatrices() { return shadowProj; }
	inline Matrix4* GetShadowProjViewMatrices() { return shadowProjView; }

	inline Vector3& GetAmbientColor() { return ambientColor; }
	inline Vector3& GetLightDirection() { return lightDirection; }
	inline float& GetSpecularFactor() { return specularFactor; }
	inline GLuint& GetShadowTex() { return shadowTex; }

protected:
	GraphicsPipeline();
	virtual ~GraphicsPipeline();

	void Resize(int x, int y); //Called by window when it is resized

	void LoadShaders();
	void UpdateAssets(int width, int height);
	void BuildAndSortRenderLists();
	void RecursiveAddToRenderLists(RenderNode* node);
	void RenderAllObjects(bool isShadowPass, std::function<void(RenderNode*)> perObjectFunc = NULL);
	void BuildShadowTransforms(); //Builds the shadow projView matrices

protected:

	RenderBase* renderer = nullptr;


	Matrix4 projViewMatrix;

	//Render FBO
	GLuint				screenTexWidth, screenTexHeight;
	GLuint				screenFBO;
	GLuint				screenTexColor;
	GLuint				screenTexDepth;

	//Shaders
	Shader* shaderPresentToWindow;
	Shader* shaderShadow;
	Shader* shaderForwardLighting;

	//Render Params
	Vector3	ambientColor;
	float	gammaCorrection;	//Monitor Default: 1.0 / 2.2 (Where 2.2 here is the gamma of the monitor which we need to invert before doing lighting calculations)		
	Vector3	lightDirection;
	Vector3 backgroundColor;
	float	specularFactor;
	uint	numSuperSamples;


	//Shadowmaps
	float	sceneBoundingRadius; ///Approx based on scene contents
	GLuint	shadowFBO;
	GLuint	shadowTex;
	Matrix4	shadowProj[SHADOWMAP_NUM];
	Matrix4	shadowViewMtx;
	Matrix4	shadowProjView[SHADOWMAP_NUM];
	float   normalizedFarPlanes[SHADOWMAP_NUM - 1];

	//Common
	Mesh* fullscreenQuad;
	Camera* camera;
	bool isVsyncEnabled;
	std::vector<RenderNode*> allNodes;

	std::vector<RenderNode*> renderlistOpaque;
	std::vector<TransparentPair> renderlistTransparent;	//Also stores cameraDist in the second argument for sorting purposes
};
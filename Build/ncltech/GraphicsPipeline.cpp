#include "GraphicsPipeline.h"
#include "ScreenPicker.h"
#include "BoundingBox.h"
#include <nclgl\NCLDebug.h>
#include <algorithm>
#include <nclgl\Graphics\Renderer\RenderFactory.h>
#include <nclgl\Graphics\Renderer\ShaderFactory.h>
#include <nclgl\Graphics\Renderer\RenderNodeBase.h>
#include <nclgl\Graphics\Renderer\OpenGL\OGLMesh.h>
#include <nclgl\Graphics\ShaderBase.h>
#include <nclgl\Graphics\TextureBase.h>
#include <nclgl\Graphics\FrameBufferBase.h>
#include <nclgl\Graphics\Renderer\FrameBufferFactory.h>
#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>
#include "Player.h"
#include "SceneManager.h"
#include <nclgl\GameLogic.h>
#include <nclgl\ResourceManager.h>

using namespace nclgl::Maths;

GraphicsPipeline::GraphicsPipeline()
	: camera(new Camera())
	, isVsyncEnabled(false)
	, screenTexWidth(0)
	, screenTexHeight(0)
	, screenFBO(NULL)
	, shaderPresentToWindow(NULL)
	, shaderShadow(NULL)
	, shaderForwardLighting(NULL)
	, fullscreenQuad(NULL)
	, shadowFBO(NULL)
	, TrailBuffer(NULL)
{
	renderer = RenderFactory::Instance()->MakeRenderer();

	LoadShaders();
	NCLDebug::_LoadShaders();
	
	trailQuad = OGLMesh::GenerateQuad();
	minimap = OGLMesh::GenerateQuad();
	
	tempProj = renderer->GetProjMatrix();
	tempView = renderer->GetViewMatrix();
	fullscreenQuad = OGLMesh::GenerateQuad();

	renderer->SetDefaultSettings();

	sceneBoundingRadius = 30.f; ///Approx based on scene contents

	InitializeDefaults();
	

	memset(world_paint, 0, sizeof(world_paint[0][0]) * GROUND_TEXTURE_SIZE * GROUND_TEXTURE_SIZE);
	paint_perc = 0.0f;

	ResourceManager::Instance()->MakeTexture("gr_tex",Texture::COLOUR, 2048,2048);
	ResourceManager::Instance()->MakeTexture("circle_tex", Texture::COLOUR, 2048, 2048);
	
	TextureBase* depth = NULL;
	TrailBuffer = FrameBufferFactory::Instance()->MakeFramebuffer(ResourceManager::Instance()->getTexture("gr_tex"), depth);
	CircleBuffer = FrameBufferFactory::Instance()->MakeFramebuffer(ResourceManager::Instance()->getTexture("circle_tex"), depth);

	minimap->SetTexture(ResourceManager::Instance()->getTexture("gr_tex"));

	Resize(renderer->GetWidth(), renderer->GetHeight());
	temp_tex = TextureFactory::Instance()->MakeTexture(TEXTUREDIR"Background.jpg");
}

GraphicsPipeline::~GraphicsPipeline()
{  
	//SAFE_DELETE(camera);
	for (int i = 0; i < cameras.size(); i++) {
		SAFE_DELETE(cameras[i]);
	}
	
	SAFE_DELETE(fullscreenQuad);

	SAFE_DELETE(shaderPresentToWindow);
	SAFE_DELETE(shaderShadow);
	SAFE_DELETE(shaderForwardLighting);

	NCLDebug::_ReleaseShaders();
	SAFE_DELETE(screenFBO);
	SAFE_DELETE(shadowFBO);
}

void GraphicsPipeline::InitializeDefaults()
{
	allNodes.clear();
	ScreenPicker::Instance()->ClearAllObjects();

	backgroundColor = Vector3(0.8f, 0.8f, 0.8f);
	ambientColor = Vector3(0.2f, 0.2f, 0.2f);
	lightDirection = Vector3(0.5f, -1.0f, -0.8f); lightDirection.Normalise();
	specularFactor = 96.0f;

	numSuperSamples = 4;
	gammaCorrection = 1.0f / 2.2f;
}


void GraphicsPipeline::AddRenderNode(RenderNodeBase* node)
{
	if (std::find(allNodes.begin(), allNodes.end(), node) == allNodes.end())
		allNodes.push_back(node);
}

void GraphicsPipeline::RemoveRenderNode(RenderNodeBase* node)
{
	allNodes.erase(std::remove(allNodes.begin(), allNodes.end(), node), allNodes.end());
}

void GraphicsPipeline::LoadShaders()
{
	shaderTrail = ShaderFactory::Instance()->MakeShader(
		SHADERDIR"SceneRenderer/testvertex.glsl",
		SHADERDIR"SceneRenderer/testfrag.glsl");
	
	shaderCircle = ShaderFactory::Instance()->MakeShader(
		SHADERDIR"SceneRenderer/testvertex.glsl",
		SHADERDIR"SceneRenderer/circlefrag.glsl");

	shaderPresentToWindow = ShaderFactory::Instance()->MakeShader(
		SHADERDIR"SceneRenderer/TechVertexBasic.glsl",
		SHADERDIR"SceneRenderer/TechFragSuperSample.glsl");

	shaderShadow = ShaderFactory::Instance()->MakeShader(
		SHADERDIR"SceneRenderer/TechVertexShadow.glsl",
		SHADERDIR"Common/EmptyFragment.glsl",
		SHADERDIR"SceneRenderer/TechGeomShadow.glsl");

	shaderForwardLighting = ShaderFactory::Instance()->MakeShader(
		SHADERDIR"SceneRenderer/TechVertexFull.glsl",
		SHADERDIR"SceneRenderer/TechFragForwardRender.glsl");
}

void GraphicsPipeline::UpdateAssets(int width, int height)
{
	//Screen Framebuffer
	if (width * numSuperSamples != screenTexWidth || height * numSuperSamples != screenTexHeight)
	{
		screenTexWidth = (uint)(width * numSuperSamples);
		screenTexHeight = (uint)(height * numSuperSamples);
		ScreenPicker::Instance()->UpdateAssets(screenTexWidth, screenTexHeight);

		//Color Texture
		ResourceManager::Instance()->MakeTexture("screenTexColor",Texture::COLOUR, screenTexWidth, screenTexHeight);
		//Depth Texture
		ResourceManager::Instance()->MakeTexture("screenTexDepth",Texture::DEPTH, screenTexWidth, screenTexHeight);
		//Generate our Framebuffer
		screenFBO = FrameBufferFactory::Instance()->MakeFramebuffer(ResourceManager::Instance()->getTexture("screenTexColor"), ResourceManager::Instance()->getTexture("screenTexDepth"));
	}
	
	//Construct our Shadow Maps and Shadow UBO
	ResourceManager::Instance()->MakeTexture("shadowTex",Texture::DEPTH_ARRAY, SHADOWMAP_SIZE, SHADOWMAP_NUM);
	shadowFBO = FrameBufferFactory::Instance()->MakeFramebuffer(ResourceManager::Instance()->getTexture("shadowTex"), false);

	//m_ShadowUBO._ShadowMapTex = glGetTextureHandleARB(m_ShadowTex);
	//glMakeTextureHandleResidentARB(m_ShadowUBO._ShadowMapTex);
}


void GraphicsPipeline::UpdateScene(float dt)
{
	if (!ScreenPicker::Instance()->HandleMouseClicks(dt))
		camera->HandleMouse(dt);

	for (int i = 0; i < cameras.size(); i++) {
		cameras[i]->HandleKeyboard(dt);
		viewMatrices[i] = cameras[i]->BuildViewMatrix();
		projViewMatrices[i] = renderer->GetProjMatrix() * viewMatrices[i];
	}

	NCLDebug::_SetDebugDrawData(
		renderer->GetProjMatrix(),
		renderer->GetViewMatrix(),
		camera->GetPosition());
}

void GraphicsPipeline::RenderMenu() {
	//Build World Transforms
	// - Most scene objects will probably end up being static, so we really should only be updating
	//   modelMatrices for objects (and their children) who have actually moved since last frame
	//for (RenderNodeBase* node : allNodes)
	//	node->Update(0.0f); //Not sure what the msec is here is for, apologies if this breaks anything in your framework!

							//Build Transparent/Opaque Renderlists


	BuildAndSortRenderLists();

	//NCLDebug - Build render lists
	NCLDebug::_BuildRenderLists();


	//Build shadowmaps
	//BuildShadowTransforms();
	shadowFBO->Activate();
	renderer->SetViewPort(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	renderer->Clear(Renderer::DEPTH);

	shaderShadow->Activate();
	shaderShadow->SetUniform("uShadowTransform[0]", SHADOWMAP_NUM, shadowProjView);

	RenderAllObjects(true,
		[&](RenderNodeBase* node)
	{
		shaderShadow->SetUniform("uModelMtx", node->GetWorldTransform());
	}
	);

	//Render scene to screen fbo
	screenFBO->Activate();
	renderer->SetViewPort(screenTexWidth, screenTexHeight);
	renderer->SetClearColour(backgroundColor);
	renderer->Clear(Renderer::COLOUR_DEPTH);

	shaderForwardLighting->Activate();
	shaderForwardLighting->SetUniform("uProjViewMtx", projViewMatrix);
	shaderForwardLighting->SetUniform("uDiffuseTex", 0);
	shaderForwardLighting->SetUniform("uCameraPos", camera->GetPosition());
	shaderForwardLighting->SetUniform("uAmbientColor", ambientColor);
	shaderForwardLighting->SetUniform("uLightDirection", lightDirection);
	shaderForwardLighting->SetUniform("uSpecularFactor", specularFactor);
	//shaderForwardLighting->SetUniform("uShadowTransform[0]", SHADOWMAP_NUM, shadowProjView);
	//shaderForwardLighting->SetUniform("uShadowTex", 2);
	//shaderForwardLighting->SetUniform("uShadowSinglePixel", Vector2(1.f / SHADOWMAP_SIZE, 1.f / SHADOWMAP_SIZE));

	//ResourceManager::Instance()->getTexture("shadowTex")->Bind(2);

	RenderAllObjects(false,
		[&](RenderNodeBase* node)
	{
		shaderForwardLighting->SetUniform("uModelMtx", node->GetWorldTransform());
		shaderForwardLighting->SetUniform("uColor", node->GetColour());
	}
	);

	// Render Screen Picking ID's
	// - This needs to be somewhere before we lose our depth buffer
	//   BUT at the moment that means our screen picking is super sampled and rendered at 
	//   a much higher resolution. Which is silly.
	ScreenPicker::Instance()->RenderPickingScene(projViewMatrix, Matrix4::Inverse(projViewMatrix), ResourceManager::Instance()->getTexture("screenTexDepth")->TempGetID(), screenTexWidth, screenTexHeight);

	screenFBO->Activate();
	renderer->SetViewPort(screenTexWidth, screenTexHeight);
	//NCLDEBUG - World Debug Data (anti-aliased)		
	NCLDebug::_RenderDebugDepthTested();
	NCLDebug::_RenderDebugNonDepthTested();



	//Downsample and present to screen
	renderer->BindScreenFramebuffer();
	renderer->SetViewPort(renderer->GetWidth(), renderer->GetHeight());
	renderer->Clear(Renderer::COLOUR_DEPTH);

	float superSamples = (float)(numSuperSamples);
	shaderPresentToWindow->Activate();
	shaderPresentToWindow->SetUniform("uColorTex", 0);
	shaderPresentToWindow->SetUniform("uGammaCorrection", gammaCorrection);
	shaderPresentToWindow->SetUniform("uNumSuperSamples", superSamples);
	shaderPresentToWindow->SetUniform("uSinglepixel", Vector2(1.f / screenTexWidth, 1.f / screenTexHeight));
	 
	fullscreenQuad->SetTexture(temp_tex);
	fullscreenQuad->Draw();

	//NCLDEBUG - Text Elements (aliased)
	NCLDebug::_RenderDebugClipSpace();
	NCLDebug::_ClearDebugLists();
	
	renderer->SwapBuffers();
	
}

void GraphicsPipeline::RenderScene()
{
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_C) && GameLogic::Instance()->getNumPlayers()>1) {

		minimap->ReplaceTexture(ResourceManager::Instance()->getTexture("circle_tex"),0);
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_Z)) {
		minimap->ReplaceTexture(ResourceManager::Instance()->getTexture("gr_tex"),0);

	}
	RenderNodeBase * ground = NULL;
	for (RenderNodeBase* node : allNodes) {
		node->Update(0.0f); //Not sure what the msec is here is for, apologies if this breaks anything in your framework!
		if ((*node->GetChildIteratorStart())->HasTag(Tags::TGround)) {
			ground = (*node->GetChildIteratorStart());
		}
	}
	ground->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture("gr_tex"), 1);
	for (int i = 0; i < cameras.size(); i++) {
		camera = cameras[i];
		projViewMatrix = projViewMatrices[i];

		//Build World Transforms
		// - Most scene objects will probably end up being static, so we really should only be updating
		//   modelMatrices for objects (and their children) who have actually moved since last frame
		


		GameLogic::Instance()->calculatePaintPercentage();

		SceneManager::Instance()->GetCurrentScene()->Score = (*GameLogic::Instance()->getPaintPerc())[0];

		TrailBuffer->Activate();
		renderer->SetViewPort(2048, 2048);

		shaderTrail->Activate();
		shaderTrail->SetUniform("num_players", GameLogic::Instance()->getNumPlayers());

		for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); i++) {
			std::string arr = "players[" + std::to_string(i) + "].";
			float pos_x = GameLogic::Instance()->getPlayer(i)->getRelativePosition().x;
			float pos_z = GameLogic::Instance()->getPlayer(i)->getRelativePosition().z;
			float rad = GameLogic::Instance()->getPlayer(i)->getRadius();
			Vector4 temp_col = (*GameLogic::Instance()->getPlayer(i)->Render()->GetChildIteratorStart())->GetColour();
			Vector3 trailColor = Vector3(temp_col.x, temp_col.y, temp_col.z);
			
			shaderTrail->SetUniform((arr + "pos_x").c_str(), pos_x);
			shaderTrail->SetUniform((arr + "pos_z").c_str(), pos_z);
			shaderTrail->SetUniform((arr + "rad").c_str(), rad);
			shaderTrail->SetUniform((arr + "trailColor").c_str(), trailColor);

		}

		trailQuad->Draw();

	

		CircleBuffer->Activate();
		renderer->SetViewPort(2048, 2048);
		shaderCircle->Activate();
		shaderCircle->SetUniform("num_players", GameLogic::Instance()->getNumPlayers());
		float sum_score = 0.0f;
		float angle = 0.0f;
		for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); i++) {
			sum_score += (*GameLogic::Instance()->getPaintPerc())[i];
		}
		for (int i = 0; i < GameLogic::Instance()->getNumPlayers()-1; i++) {
			std::string arr = "players[" + std::to_string(i) + "].";
			angle += 2*PI*(*GameLogic::Instance()->getPaintPerc())[i] / sum_score;
			shaderCircle->SetUniform((arr + "angle").c_str(), angle);
		}

		trailQuad->Draw();


		//Build Transparent/Opaque Renderlists
		BuildAndSortRenderLists();
		//Build World Transforms
		// - Most scene objects will probably end up being static, so we really should only be updating
		//   modelMatrices for objects (and their children) who have actually moved since last frame
		for (RenderNodeBase* node : allNodes)
			node->Update(0.0f); //Not sure what the msec is here is for, apologies if this breaks anything in your framework!


		//NCLDebug - Build render lists
		NCLDebug::_BuildRenderLists();

		//Build shadowmaps
		BuildShadowTransforms();
		shadowFBO->Activate();
		renderer->SetViewPort(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
		renderer->Clear(Renderer::DEPTH);

		shaderShadow->Activate();
		shaderShadow->SetUniform("uShadowTransform[0]", SHADOWMAP_NUM, shadowProjView);

		RenderAllObjects(true,

			[&](RenderNodeBase* node)
		{
			shaderShadow->SetUniform("uModelMtx", node->GetWorldTransform());
		}
		);

		//Render scene to screen fbo
		screenFBO->Activate();
		renderer->SetViewPort(screenTexWidth, screenTexHeight);
		renderer->SetClearColour(backgroundColor);
		renderer->Clear(Renderer::COLOUR_DEPTH);

		shaderForwardLighting->Activate();
		shaderForwardLighting->SetUniform("uProjViewMtx", projViewMatrix);

		shaderForwardLighting->SetUniform("uDiffuseTex0", 0);
		shaderForwardLighting->SetUniform("uDiffuseTex1", 1);


		shaderForwardLighting->SetUniform("uCameraPos", camera->GetPosition());
		shaderForwardLighting->SetUniform("uAmbientColor", ambientColor);
		shaderForwardLighting->SetUniform("uLightDirection", lightDirection);
		shaderForwardLighting->SetUniform("uSpecularFactor", specularFactor);
		shaderForwardLighting->SetUniform("uShadowTransform[0]", SHADOWMAP_NUM, shadowProjView);
		shaderForwardLighting->SetUniform("uShadowTex", 2);
		shaderForwardLighting->SetUniform("uShadowSinglePixel", Vector2(1.f / SHADOWMAP_SIZE, 1.f / SHADOWMAP_SIZE));

		ResourceManager::Instance()->getTexture("shadowTex")->Bind(2);

		RenderAllObjects(false,

			[&](RenderNodeBase* node)
		{
			shaderForwardLighting->SetUniform("uModelMtx", node->GetWorldTransform());
			shaderForwardLighting->SetUniform("uColor", node->GetColour());
		}
		);

		// Render Screen Picking ID's
		// - This needs to be somewhere before we lose our depth buffer
		//   BUT at the moment that means our screen picking is super sampled and rendered at 
		//   a much higher resolution. Which is silly.
		ScreenPicker::Instance()->RenderPickingScene(projViewMatrix, Matrix4::Inverse(projViewMatrix), ResourceManager::Instance()->getTexture("screenTexDepth")->TempGetID(), screenTexWidth, screenTexHeight);

		screenFBO->Activate();
		renderer->SetViewPort(screenTexWidth, screenTexHeight);

		//NCLDEBUG - World Debug Data (anti-aliased)		
		NCLDebug::_RenderDebugDepthTested();
		NCLDebug::_RenderDebugNonDepthTested();



		//Downsample and present to screen



		for (int j = 0; j < 2; j++) {
			renderer->BindScreenFramebuffer();
			
			renderer->SetScissor(TRUE);
			AdjustViewport(i, j);
			renderer->Clear(Renderer::COLOUR_DEPTH);
			renderer->SetScissor(FALSE);
			//Downsample and present to screen

			float superSamples = (float)(numSuperSamples);
			shaderPresentToWindow->Activate();
			shaderPresentToWindow->SetUniform("uColorTex", 0);
			shaderPresentToWindow->SetUniform("uGammaCorrection", gammaCorrection);
			shaderPresentToWindow->SetUniform("uNumSuperSamples", superSamples);
			shaderPresentToWindow->SetUniform("uSinglepixel", Vector2(1.f / screenTexWidth, 1.f / screenTexHeight));

			fullscreenQuad->ReplaceTexture(ResourceManager::Instance()->getTexture("screenTexColor"),0);


			if (j == 0) {

				fullscreenQuad->Draw();
			}
			else {
				tempProj = renderer->GetProjMatrix();
				tempView = renderer->GetViewMatrix();
				renderer->SetProjMatrix(Matrix4::Orthographic(-1, 1, 1, -1, -1, 1));
				renderer->GetViewMatrix().ToIdentity();

				minimap->Draw();
				renderer->SetProjMatrix(tempProj);
				renderer->SetViewMatrix(tempView);
			}


			//NCLDEBUG - Text Elements (aliased)
			NCLDebug::_RenderDebugClipSpace();
			NCLDebug::_ClearDebugLists();
		}


		
	}
	renderer->SwapBuffers();
}

void GraphicsPipeline::AdjustViewport(int i, int j) {
	float width = renderer->GetWidth();
	float height = renderer->GetHeight();
	int num_p = GameLogic::Instance()->getNumPlayers();
	if (j == 0) {
		if (num_p == 1) {
			renderer->SetViewPort(width, height);
		}
		else if (num_p == 2) {
			renderer->SetProjMatrix(Matrix4::Perspective(PROJ_NEAR, PROJ_FAR, ((float)width) / ((float)height / 2.0f), PROJ_FOV));
			if (i == 0) {
				renderer->SetViewPort(0, height / 2, width, height / 2);
				renderer->Scissor(0, height / 2, width, height / 2);
			}
			else {
				renderer->SetViewPort(0, 0, width, height / 2);
				renderer->Scissor(0, 0, width, height / 2);
			}
		}
		else if (num_p == 3) {
			if (i == 0) {
				renderer->SetViewPort(0, height / 2, width / 2, height / 2);
				renderer->Scissor(0, height / 2, width / 2, height / 2);
			}
			else if (i == 1) {
				renderer->SetViewPort(0, 0, width / 2, height / 2);
				renderer->Scissor(0, 0, width / 2, height / 2);
			}
			else {
				renderer->SetViewPort(width / 2, 0, width / 2, height / 2);
				renderer->Scissor(width / 2, 0, width / 2, height / 2);
			}
		}
		else if (num_p == 4) {
			if (i == 0) {
				renderer->SetViewPort(0, height / 2, width / 2, height / 2);
				renderer->Scissor(0, height / 2, width / 2, height / 2);
			}
			else if (i == 1) {
				renderer->SetViewPort(width / 2, height / 2, width / 2, height / 2);
				renderer->Scissor(width / 2, height / 2, width / 2, height / 2);
			}
			else if (i == 2) {
				renderer->SetViewPort(0, 0, width / 2, height / 2);
				renderer->Scissor(0, 0, width / 2, height / 2);
			}
			else {
				renderer->SetViewPort(width / 2, 0, width / 2, height / 2);
				renderer->Scissor(width / 2, 0, width / 2, height / 2);
			}
		}
	}
	else {
		if (num_p == 1) {
			renderer->Scissor(4 * width / 5, 0, width / 5, width / 5);
			renderer->SetViewPort(4 * width / 5, 0, width / 5, width / 5);
		}
		else if (num_p == 2) {
			renderer->Scissor(4 * width / 5, height/2 - width/10, width / 5, width / 5);
			renderer->SetViewPort(4 * width / 5, height / 2 - width / 10, width / 5, width / 5);
		}
		else if (num_p == 3) {
			renderer->SetViewPort(width / 2, height / 2, width / 2, height / 2);
			renderer->Scissor(width / 2, height / 2, width / 2, height / 2);
		}
		else if (num_p == 4) {
			renderer->Scissor(3 * width / 7, height / 2 - width / 14, width / 7, width / 7);
			renderer->SetViewPort(3 * width / 7, height / 2 - width / 14, width / 7, width / 7);
		}
	}
}

void GraphicsPipeline::Resize(int x, int y)
{
	if (x <= 0 || y <= 0)
	{
		x = 1;
		y = 1;
	}
	
	//Generate/Resize any screen textures (if needed)
	UpdateAssets(x, y);

	//Update 'width', 'height' vars
	renderer->Resize(x, y);

	//Update our projection matrix
	renderer->SetProjMatrix(Matrix4::Perspective(PROJ_NEAR, PROJ_FAR, (float)x / (float)y, PROJ_FOV));
}

void GraphicsPipeline::BuildAndSortRenderLists()
{
	//Divide 'nodesAll' into transparent and opaque versions
	// - As objects don't change their color/transparency that often
	//   this could be improved by just directly inserting into the given list
	//   and maintaining a permenantly sorted list of transparent objects.
	renderlistOpaque.clear();
	renderlistTransparent.clear();

	for (RenderNodeBase* node : allNodes)
		RecursiveAddToRenderLists(node);
	
	//Sort transparent objects back to front
	std::sort(
		renderlistTransparent.begin(),
		renderlistTransparent.end(),
		[](const TransparentPair& a, const TransparentPair& b)
		{
			return a.second > b.second;
		}
	);
}

void GraphicsPipeline::RecursiveAddToRenderLists(RenderNodeBase* node)
{
	//If the node is renderable, add it to either a opaque or transparent render list
	if (node->IsRenderable())
	{
		if (node->GetColour().w > 0.999f)
		{
			renderlistOpaque.push_back(node);
		}
		else
		{
			Vector3 diff = node->GetWorldTransform().GetPositionVector() - camera->GetPosition();
			float camDistSq = Vector3::Dot(diff, diff); //Same as doing .Length() without the sqrt

			renderlistTransparent.push_back({ node, camDistSq });
		}
	}

	//Recurse over all children and process them aswell
	for (auto itr = node->GetChildIteratorStart(); itr != node->GetChildIteratorEnd(); itr++)
		RecursiveAddToRenderLists(*itr);
}

void GraphicsPipeline::RenderAllObjects(bool isShadowPass, std::function<void(RenderNodeBase*)> perObjectFunc)
{
	for (RenderNodeBase* node : renderlistOpaque)
	{
		perObjectFunc(node);
		node->Draw();
	}

	if (isShadowPass)
	{
		for (TransparentPair& node : renderlistTransparent)
		{
			perObjectFunc(node.first);
			node.first->Draw();
		}
	}
	else
	{
		for (TransparentPair& node : renderlistTransparent)
		{
			perObjectFunc(node.first);
			renderer->SetScreenCulling(Renderer::FRONT);
			node.first->Draw();

			renderer->SetScreenCulling(Renderer::BACK);
			node.first->Draw();
		}
	}
}

void GraphicsPipeline::BuildShadowTransforms()
{
	const float proj_range = PROJ_FAR - PROJ_NEAR;

	//Variable size - shadowmap always rotated to be square with camera
	//  Vector3 viewDir = Matrix3::Transpose(Matrix3(viewMatrix)) * Vector3(0, 0, 1);
	//  Matrix4 lightview = Matrix4::BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), -lightDirection, viewDir);	

	//Fixed size shadow area (just moves with camera) 
	shadowViewMtx = Matrix4::BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), -lightDirection, Vector3(0, 1, 0));

	Matrix4 invCamProjView = Matrix4::Inverse(renderer->GetProjMatrix() * renderer->GetViewMatrix());

	auto compute_depth = [&](float x)
	{
		float proj_start = -(proj_range * x + PROJ_NEAR);
		return (proj_start*renderer->GetProjMatrix()[10] + renderer->GetProjMatrix()[14]) / (proj_start*renderer->GetProjMatrix()[11]);
	};

	const float divisor = (SHADOWMAP_NUM*SHADOWMAP_NUM) - 1.f;
	for (int i = 0; i < SHADOWMAP_NUM; ++i)
	{
		//Linear scalars going from 0.0f (near) to 1.0f (far)
		float lin_near = (powf(2.0f, (float)(i)) - 1.f) / divisor;
		float lin_far = (powf(2.0f, (float)(i + 1)) - 1.f) / divisor;

		//True non-linear depth ranging from -1.0f (near) to 1.0f (far)
		float norm_near = compute_depth(lin_near);
		float norm_far = compute_depth(lin_far);
		
		//Build Bounding Box around frustum section (Axis Aligned)
		BoundingBox bb;
		bb.ExpandToFit(invCamProjView * Vector3(-1.0f, -1.0f, norm_near));
		bb.ExpandToFit(invCamProjView * Vector3(-1.0f,  1.0f, norm_near));
		bb.ExpandToFit(invCamProjView * Vector3( 1.0f, -1.0f, norm_near));
		bb.ExpandToFit(invCamProjView * Vector3( 1.0f,  1.0f, norm_near));
		bb.ExpandToFit(invCamProjView * Vector3(-1.0f, -1.0f, norm_far));
		bb.ExpandToFit(invCamProjView * Vector3(-1.0f,  1.0f, norm_far));
		bb.ExpandToFit(invCamProjView * Vector3( 1.0f, -1.0f, norm_far));
		bb.ExpandToFit(invCamProjView * Vector3( 1.0f,  1.0f, norm_far));
		
		//Rotate bounding box so it's orientated in the lights direction
		// - Rotates bounding box and creates a new AABB that encompasses it
		bb = bb.Transform(shadowViewMtx);

		//Extend the Z depths to catch shadow casters outside view frustum
		bb._min.z = min(bb._min.z, -sceneBoundingRadius);
		bb._max.z = max(bb._max.z, sceneBoundingRadius);

		//Build Light Projection		
		shadowProj[i] = Matrix4::Orthographic(bb._max.z, bb._min.z, bb._min.x, bb._max.x, bb._max.y, bb._min.y);
		shadowProjView[i] = shadowProj[i] * shadowViewMtx;
	}
}

Camera* GraphicsPipeline::CreateNewCamera() {
	Camera* cam = new Camera();
	cameras.push_back(cam);
	viewMatrices.push_back(cam->BuildViewMatrix());
	projViewMatrices.push_back(renderer->GetProjMatrix());
	cam->SetPitch(-20.0f);
	return cam;
}

void GraphicsPipeline::ChangeScene() {
	renderer->SwapBuffers();
	cameras.clear();
	screenFBO->Activate();
	NCLDebug::_ClearDebugLists();
	NCLDebug::_ReleaseShaders();
	renderer->BindScreenFramebuffer();
	renderer->Clear(Renderer::COLOUR_DEPTH);
}
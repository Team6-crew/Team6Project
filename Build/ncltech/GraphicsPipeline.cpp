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
	, shaderPaint(NULL)
	, fullscreenQuad(NULL)
	, shadowFBO(NULL)
	, TrailBuffer(NULL)
	, ground(NULL)
	, flickerPie(0.0f)
	, toggleFlicker(false)
{
	renderer = RenderFactory::Instance()->MakeRenderer();
	LoadShaders();
	NCLDebug::_LoadShaders();

	trailQuad = OGLMesh::GenerateQuad();
	minimap = OGLMesh::GenerateQuad();
	piemap = OGLMesh::GenerateQuad();
	tempProj = renderer->GetProjMatrix();
	tempView = renderer->GetViewMatrix();
	fullscreenQuad = OGLMesh::GenerateQuad();
	paintQuad = OGLMesh::GenerateQuad();
	ResourceManager::Instance()->MakeTexture("temp_tex", Texture::COLOUR, 1, 1);
	renderer->SetDefaultSettings();

	sceneBoundingRadius = 30.f; ///Approx based on scene contents

	InitializeDefaults();
	sound321played = false;

	memset(world_paint, 0, sizeof(world_paint[0][0]) * GROUND_TEXTURE_SIZE * GROUND_TEXTURE_SIZE);
	paint_perc = 0.0f;

	ResourceManager::Instance()->MakeTexture("gr_tex",Texture::COLOUR, 2048,2048);
	ResourceManager::Instance()->MakeTexture("paintable_tex", Texture::COLOUR, 2048, 2048);
	ResourceManager::Instance()->MakeTexture("circle_tex", Texture::COLOUR, 2048, 2048);
	loading_tex = TextureFactory::Instance()->MakeTexture(TEXTUREDIR"loading.png");
	TextureBase* depth = NULL;
	TrailBuffer = FrameBufferFactory::Instance()->MakeFramebuffer(ResourceManager::Instance()->getTexture("gr_tex"), depth);
	CircleBuffer = FrameBufferFactory::Instance()->MakeFramebuffer(ResourceManager::Instance()->getTexture("circle_tex"), depth);
	PaintBuffer = FrameBufferFactory::Instance()->MakeFramebuffer(ResourceManager::Instance()->getTexture("temp_tex"), depth);

	minimap->SetTexture(ResourceManager::Instance()->getTexture("gr_tex"));
	piemap->SetTexture(ResourceManager::Instance()->getTexture("circle_tex"));
	Resize(renderer->GetWidth(), renderer->GetHeight());
	temp_tex = TextureFactory::Instance()->MakeTexture(TEXTUREDIR"Background.jpg");
	splat_tex = TextureFactory::Instance()->MakeTexture(TEXTUREDIR"splat.png");
	tex_1 = TextureFactory::Instance()->MakeTexture(TEXTUREDIR"1.png");
	tex_2 = TextureFactory::Instance()->MakeTexture(TEXTUREDIR"2.png");
	tex_3 = TextureFactory::Instance()->MakeTexture(TEXTUREDIR"3.png");
	buff_paint_tex = TextureFactory::Instance()->MakeTexture(TEXTUREDIR"paint.png");
	buff_speed_tex = TextureFactory::Instance()->MakeTexture(TEXTUREDIR"speed.png");
	buff_stun_tex = TextureFactory::Instance()->MakeTexture(TEXTUREDIR"stun.png");

	BuffBuffer = FrameBufferFactory::Instance()->MakeFramebuffer(tex_3, depth);
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

	shaderPaint = ShaderFactory::Instance()->MakeShader(
		SHADERDIR"SceneRenderer/testvertex.glsl",
		SHADERDIR"SceneRenderer/PaintFrag.glsl");

	shaderLoading = ShaderFactory::Instance()->MakeShader(
		SHADERDIR"SceneRenderer/testvertex.glsl",
		SHADERDIR"SceneRenderer/loadingFrag.glsl");

	shaderSplat = ShaderFactory::Instance()->MakeShader(
		SHADERDIR"SceneRenderer/testvertex.glsl",
		SHADERDIR"SceneRenderer/SplatFrag.glsl");

	shaderPaintable = ShaderFactory::Instance()->MakeShader(
		SHADERDIR"SceneRenderer/testvertex.glsl",
		SHADERDIR"SceneRenderer/PaintableFrag.glsl");

	shaderMap = ShaderFactory::Instance()->MakeShader(
		SHADERDIR"SceneRenderer/testvertex.glsl",
		SHADERDIR"SceneRenderer/MapFrag.glsl");

	shaderCounter = ShaderFactory::Instance()->MakeShader(
		SHADERDIR"SceneRenderer/testvertex.glsl",
		SHADERDIR"SceneRenderer/CounterFrag.glsl");

	shaderBuff = ShaderFactory::Instance()->MakeShader(
		SHADERDIR"SceneRenderer/testvertex.glsl",
		SHADERDIR"SceneRenderer/BuffFrag.glsl");
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
		ResourceManager::Instance()->MakeTexture("screenTexColor", Texture::COLOUR, screenTexWidth, screenTexHeight);
		//Depth Texture
		ResourceManager::Instance()->MakeTexture("screenTexDepth", Texture::DEPTH, screenTexWidth, screenTexHeight);
		//Generate our Framebuffer
		screenFBO = FrameBufferFactory::Instance()->MakeFramebuffer(ResourceManager::Instance()->getTexture("screenTexColor"), ResourceManager::Instance()->getTexture("screenTexDepth"));
	}

	//Construct our Shadow Maps and Shadow UBO
	ResourceManager::Instance()->MakeTexture("shadowTex", Texture::DEPTH_ARRAY, SHADOWMAP_SIZE, SHADOWMAP_NUM);
	shadowFBO = FrameBufferFactory::Instance()->MakeFramebuffer(ResourceManager::Instance()->getTexture("shadowTex"), false);

	//m_ShadowUBO._ShadowMapTex = glGetTextureHandleARB(m_ShadowTex);
	//glMakeTextureHandleResidentARB(m_ShadowUBO._ShadowMapTex);
}


void GraphicsPipeline::UpdateScene(float dt)
{

	

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
	 
	fullscreenQuad->ReplaceTexture(temp_tex,0);
	fullscreenQuad->Draw();

	//NCLDEBUG - Text Elements (aliased)
	NCLDebug::_RenderDebugClipSpace();
	NCLDebug::_ClearDebugLists();

	renderer->BindScreenFramebuffer();
	renderer->SwapBuffers();
	renderer->Clear(Renderer::COLOUR_DEPTH);

}

void GraphicsPipeline::StartCounter() {

	renderer->SetViewPort(renderer->GetWidth(), renderer->GetHeight());
	shaderCounter->Activate();
	float tempTime = GameLogic::Instance()->getTotalTime();
	if (tempTime <= 1.0f) {
		if (!sound321played) {
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"countdown.ogg", false);
			sound321played = true;
		}
		fullscreenQuad->ReplaceTexture(tex_3, 0);
	}
	else if (tempTime <= 2.0f) {
		fullscreenQuad->ReplaceTexture(tex_2, 0);
		tempTime -= 1.0f;
	}
	else if (tempTime <= 3.0f) {
		fullscreenQuad->ReplaceTexture(tex_1, 0);
		tempTime -= 2.0f;
	}

	shaderCounter->SetUniform("DiffuseTex", 0);
	shaderCounter->SetUniform("seconds", tempTime);
	tempProj = renderer->GetProjMatrix();
	tempView = renderer->GetViewMatrix();
	renderer->SetProjMatrix(Matrix4::Orthographic(-1, 1, 1, -1, -1, 1));
	renderer->GetViewMatrix().ToIdentity();
	fullscreenQuad->Draw();
	renderer->SetProjMatrix(tempProj);
	renderer->SetViewMatrix(tempView);

}

void GraphicsPipeline::BuffHUD(int i) {
	if (GameLogic::Instance()->getSoftPlayer(i)->getCurrentBuff() == Tags::BPaint) {
		paintQuad->ReplaceTexture(buff_paint_tex, 0);
	}
	else if (GameLogic::Instance()->getSoftPlayer(i)->getCurrentBuff() == Tags::BStun) {
		paintQuad->ReplaceTexture(buff_stun_tex, 0);
	}
	else if (GameLogic::Instance()->getSoftPlayer(i)->getCurrentBuff() == Tags::BSpeed) {
		paintQuad->ReplaceTexture(buff_speed_tex, 0);
	}
	else {
		return;
	}
	renderer->SetViewPort(3*screenTexWidth/7 , screenTexHeight - screenTexWidth / 7, screenTexWidth/7, screenTexWidth / 7);
	shaderBuff->Activate();
	shaderBuff->SetUniform("DiffuseTex", 0);
	shaderBuff->SetUniform("playerColor", GameLogic::Instance()->getSoftPlayer(i)->getColour());
	shaderBuff->SetUniform("perc", GameLogic::Instance()->getSoftPlayer(i)->getCurrentBuffTime() / GameLogic::Instance()->getSoftPlayer(i)->getBuffTime());
	tempProj = renderer->GetProjMatrix();
	tempView = renderer->GetViewMatrix();
	renderer->SetProjMatrix(Matrix4::Orthographic(-1, 1, 1, -1, -1, 1));
	renderer->GetViewMatrix().ToIdentity();
	paintQuad->Draw();
	renderer->SetProjMatrix(tempProj);
	renderer->SetViewMatrix(tempView);
}

void GraphicsPipeline::LoadingScreen(float frame) {

	renderer->SetViewPort(1024, 1024);

	shaderLoading->Activate();
	PaintBuffer->ChangeColourAttachment(loading_tex);
	paintQuad->ReplaceTexture(loading_tex, 0);
	shaderLoading->SetUniform("radius_perc", frame);
	paintQuad->Draw();

	renderer->BindScreenFramebuffer();
	renderer->SetViewPort(renderer->GetWidth(), renderer->GetHeight());
	renderer->Clear(Renderer::COLOUR_DEPTH);
	float superSamples = (float)(numSuperSamples);
	shaderPresentToWindow->Activate();
	shaderPresentToWindow->SetUniform("uColorTex", 0);
	shaderPresentToWindow->SetUniform("uGammaCorrection", gammaCorrection);
	shaderPresentToWindow->SetUniform("uNumSuperSamples", superSamples);
	shaderPresentToWindow->SetUniform("uSinglepixel", Vector2(1.f / screenTexWidth, 1.f / screenTexHeight));
	fullscreenQuad->ReplaceTexture(loading_tex,0);
	fullscreenQuad->Draw();
	renderer->SwapBuffers();
}

void GraphicsPipeline::SplatProjectile(float pos_x, float pos_z, float rad, Vector4 colour) {
	TrailBuffer->Activate();
	GameObject* grnd = SceneManager::Instance()->GetCurrentScene()->FindGameObject("Ground");
	nclgl::Maths::Vector3 gr_pos = grnd->Physics()->GetPosition();
	renderer->SetViewPort(2048, 2048);
	shaderSplat->Activate();
	splat_tex->Bind(3);
	float posX = (pos_x - gr_pos.x + WORLD_SIZE) / (WORLD_SIZE * 2);
	float posZ = 1 - (pos_z - gr_pos.z + WORLD_SIZE) / (WORLD_SIZE * 2);
	shaderSplat->SetUniform("uDiffuseTex", 3);
	shaderSplat->SetUniform("pos_x", posX);
	shaderSplat->SetUniform("pos_z", posZ);
	shaderSplat->SetUniform("rad", rad/WORLD_SIZE*2.0f);
	Vector3 trailColor = Vector3(colour.x, colour.y, colour.z);
	shaderSplat->SetUniform("trailColor", trailColor);
	trailQuad->Draw();
}

void GraphicsPipeline::RenderScene(float dt)
{   
	GameLogic::Instance()->calculatePaintPercentage();
	FillPaint(dt);
	if (paintableObjects.size() > 0) {
		renderer->SetViewPort(2048, 2048);
		PaintBuffer->ChangeColourAttachment(ResourceManager::Instance()->getTexture("paintable_tex"));
		shaderPaintable->Activate();
		paintQuad->ReplaceTexture(ResourceManager::Instance()->getTexture("paintable_tex"), 0);
		GameObject* grnd = SceneManager::Instance()->GetCurrentScene()->FindGameObject("Ground");
		nclgl::Maths::Vector3 gr_pos = grnd->Physics()->GetPosition();
		int i = 0;
		for (std::vector<GameObject*>::iterator it = paintableObjects.begin(); it != paintableObjects.end(); it++) {
			std::string arr = "objects[" + std::to_string(i) + "].";
			nclgl::Maths::Vector3 halfDims = (*(*it)->Render()->GetChildIteratorStart())->GetHalfDims();

			nclgl::Maths::Vector3 position = (*it)->Physics()->GetPosition();
			float posX = (position.x - gr_pos.x + WORLD_SIZE) / (WORLD_SIZE * 2);
			float posZ = 1 - (position.z - gr_pos.z + WORLD_SIZE) / (WORLD_SIZE * 2);
			nclgl::Maths::Vector4 objectColor = (*(*it)->Render()->GetChildIteratorStart())->GetColourFromPlayer();
			shaderPaintable->SetUniform((arr + "pos_x").c_str(), posX);
			shaderPaintable->SetUniform((arr + "pos_z").c_str(), posZ);
			shaderPaintable->SetUniform((arr + "halfdims").c_str(), nclgl::Maths::Vector2(halfDims.x/ WORLD_SIZE, halfDims.z/ WORLD_SIZE));
			shaderPaintable->SetUniform((arr + "objColor").c_str(), objectColor);
			i++;
		}
		shaderPaintable->SetUniform("num_objects", i);
		paintQuad->Draw();
	}
	TrailBuffer->Activate();
	renderer->SetViewPort(2048, 2048);
	shaderTrail->Activate();
	shaderTrail->SetUniform("num_players", GameLogic::Instance()->getTotalPlayers());
	int splatPlayer = -1;
	for (int i = 0; i < GameLogic::Instance()->getTotalPlayers(); i++) {
		if (i < GameLogic::Instance()->getNumPlayers()) {
			std::string arr = "players[" + std::to_string(i) + "].";
			float pos_x = GameLogic::Instance()->getPlayer(i)->getRelativePosition().x;
			float pos_z = GameLogic::Instance()->getPlayer(i)->getRelativePosition().z;
			float rad = GameLogic::Instance()->getPlayer(i)->getRadius();
			Vector4 temp_col = (*GameLogic::Instance()->getPlayer(i)->Render()->GetChildIteratorStart())->GetColour();
			Vector3 trailColor = Vector3(temp_col.x, temp_col.y, temp_col.z);
			shaderTrail->SetUniform((arr + "trailColor").c_str(), trailColor);
			shaderTrail->SetUniform((arr + "pos_x").c_str(), pos_x);
			shaderTrail->SetUniform((arr + "pos_z").c_str(), pos_z);
			if (rad <= 0.01f) shaderTrail->SetUniform((arr + "rad").c_str(), rad);
			else {
				shaderTrail->SetUniform((arr + "rad").c_str(), 0);
				splatPlayer = i;
			}
			trailQuad->Draw();

		}
		else {
			std::string arr = "players[" + std::to_string(i) + "].";
			float pos_x = GameLogic::Instance()->getSoftPlayer(i)->getRelativePosition().x;
			float pos_z = GameLogic::Instance()->getSoftPlayer(i)->getRelativePosition().z;
			float rad = GameLogic::Instance()->getSoftPlayer(i)->getRadius();
			Vector4 temp_col = (*GameLogic::Instance()->getSoftPlayer(i)->getBottom()->Render()->GetChildIteratorStart())->GetColour();
			Vector3 trailColor = Vector3(temp_col.x, temp_col.y, temp_col.z);
			shaderTrail->SetUniform((arr + "trailColor").c_str(), trailColor);
			shaderTrail->SetUniform((arr + "pos_x").c_str(), pos_x);
			shaderTrail->SetUniform((arr + "pos_z").c_str(), pos_z);
			if (rad <= 0.01f) shaderTrail->SetUniform((arr + "rad").c_str(), rad);
			else {
				shaderTrail->SetUniform((arr + "rad").c_str(), 0);
				splatPlayer = i;
			}
			trailQuad->Draw();
		}
	}

	if (splatPlayer != -1) {
		shaderSplat->Activate();
		splat_tex->Bind(3);
		shaderSplat->SetUniform("uDiffuseTex", 3);
		float pos_x = GameLogic::Instance()->getSoftPlayer(splatPlayer)->getRelativePosition().x;
		float pos_z = GameLogic::Instance()->getSoftPlayer(splatPlayer)->getRelativePosition().z;
		float rad = GameLogic::Instance()->getSoftPlayer(splatPlayer)->getRadius();
		Vector4 temp_col = (*GameLogic::Instance()->getSoftPlayer(splatPlayer)->getBottom()->Render()->GetChildIteratorStart())->GetColour();
		Vector3 trailColor = Vector3(temp_col.x, temp_col.y, temp_col.z);
		shaderSplat->SetUniform("pos_x", pos_x);
		shaderSplat->SetUniform("pos_z", pos_z);
		shaderSplat->SetUniform("rad", rad);
		shaderSplat->SetUniform("trailColor", trailColor);
		trailQuad->Draw();
	}


	CircleBuffer->Activate();
	renderer->SetViewPort(2048, 2048);
	shaderCircle->Activate();
	shaderCircle->SetUniform("num_players", GameLogic::Instance()->getNumSoftPlayers());
	float sum_score = 0.0f;
	float angle = 0.0f;
	for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
		sum_score += (*GameLogic::Instance()->getPaintPerc())[i];
	}
	int max_score = 0;
	float max_perc = 0.0f;
	for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
		if ((*GameLogic::Instance()->getPaintPerc())[i] > max_perc) {
			max_perc = (*GameLogic::Instance()->getPaintPerc())[i];
			max_score = i;
		}
		std::string arr = "players[" + std::to_string(i) + "].";
		angle += 2 * PI*(*GameLogic::Instance()->getPaintPerc())[i] / sum_score;
		shaderCircle->SetUniform((arr + "angle").c_str(), angle);
		shaderCircle->SetUniform((arr + "player_colour").c_str(), (*GameLogic::Instance()->getSoftPlayer(i)->getBottom()->Render()->GetChildIteratorStart())->GetColour());
	}
	
	if (flickerPie > 0.5f) {
		flickerPie = 0.0f;
		toggleFlicker = !toggleFlicker;
	}
	if (toggleFlicker) {
		max_score = -1;
	}
		int PlayersPlusAIPlayers = GameLogic::Instance()->getNumPlayers() + GameLogic::Instance()->getNumAIPlayers();
		for (int i = GameLogic::Instance()->getNumPlayers(); i < PlayersPlusAIPlayers; i++) {
			int j = i - GameLogic::Instance()->getNumPlayers();
	
			std::string arr = "players[" + std::to_string(i) + "].";
			float pos_x = GameLogic::Instance()->getAIPlayer(j)->getRelativePosition().x;
			float pos_z = GameLogic::Instance()->getAIPlayer(j)->getRelativePosition().z;
			float rad = GameLogic::Instance()->getAIPlayer(j)->getRadius();
			Vector4 temp_col = (*GameLogic::Instance()->getAIPlayer(j)->getBall()->Render()->GetChildIteratorStart())->GetColour();
			Vector3 trailColor = Vector3(temp_col.x, temp_col.y, temp_col.z);
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_C)) {
				trailColor = Vector3(0.0f, 1.0f, 0.0f);
			}
			shaderTrail->SetUniform((arr + "pos_x").c_str(), pos_x);
			shaderTrail->SetUniform((arr + "pos_z").c_str(), pos_z);
			shaderTrail->SetUniform((arr + "rad").c_str(), rad);
			shaderTrail->SetUniform((arr + "trailColor").c_str(), trailColor);

		}

	flickerPie += dt;
	shaderCircle->SetUniform("winning", max_score);
	trailQuad->Draw();

	ground->GetMesh()->ReplaceTexture(ResourceManager::Instance()->getTexture("gr_tex"), 1);
	for (int i = 0; i < cameras.size(); i++) {
		camera = cameras[i];
		projViewMatrix = projViewMatrices[i];
		BuildAndSortRenderLists();
		//Build World Transforms
		// - Most scene objects will probably end up being static, so we really should only be updating
		//   modelMatrices for objects (and their children) who have actually moved since last frame
		for (RenderNodeBase* node : allNodes)
			node->Update(0.0f); 


								//NCLDebug - Build render lists
								//NCLDebug::_BuildRenderLists();

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
		//ScreenPicker::Instance()->RenderPickingScene(projViewMatrix, Matrix4::Inverse(projViewMatrix), ResourceManager::Instance()->getTexture("screenTexDepth")->TempGetID(), screenTexWidth, screenTexHeight);

		screenFBO->Activate();
		renderer->SetViewPort(screenTexWidth, screenTexHeight);

		//NCLDEBUG - World Debug Data (anti-aliased)		
		NCLDebug::_RenderDebugDepthTested();
		NCLDebug::_RenderDebugNonDepthTested();
		BuffHUD(i);
	


		//Downsample and present to screen



		for (int j = 0; j < 3; j++) {

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

			fullscreenQuad->ReplaceTexture(ResourceManager::Instance()->getTexture("screenTexColor"), 0);


			if (j == 0) {

				fullscreenQuad->Draw();
			}
			else if (j == 1) {
				tempProj = renderer->GetProjMatrix();
				tempView = renderer->GetViewMatrix();
				renderer->SetProjMatrix(Matrix4::Orthographic(-1, 1, 1, -1, -1, 1));
				renderer->GetViewMatrix().ToIdentity();
				ResourceManager::Instance()->getTexture("paintable_tex")->Bind(3);
				shaderMap->Activate();
				shaderMap->SetUniform("uColorTex", 0);
				shaderMap->SetUniform("uColorTex2", 3);
				minimap->Draw();
				renderer->SetProjMatrix(tempProj);
				renderer->SetViewMatrix(tempView);
			}
			else {
				tempProj = renderer->GetProjMatrix();
				tempView = renderer->GetViewMatrix();
				renderer->SetProjMatrix(Matrix4::Orthographic(-1, 1, 1, -1, -1, 1));
				renderer->GetViewMatrix().ToIdentity();
				piemap->Draw();
				renderer->SetProjMatrix(tempProj);
				renderer->SetViewMatrix(tempView);
			}

		}
	}
	if (GameLogic::Instance()->getTotalTime() <= 3.0f) {
		StartCounter();
	}
	
	NCLDebug::_BuildRenderLists();
	renderer->SetViewPort(screenTexWidth / 4, screenTexHeight / 4);
	//NCLDEBUG - Text Elements (aliased)
	NCLDebug::_RenderDebugClipSpace();
	NCLDebug::_ClearDebugLists();
	screenFBO->Activate();

	renderer->Clear(Renderer::COLOUR_DEPTH);
	renderer->BindScreenFramebuffer();
	
	renderer->SwapBuffers();
	renderer->Clear(Renderer::COLOUR_DEPTH);
}

void GraphicsPipeline::AdjustViewport(int i, int j) {
	float width = renderer->GetWidth();
	float height = renderer->GetHeight();
	int num_p = GameLogic::Instance()->getTotalPlayers();
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
				renderer->SetViewPort(width / 4, height / 2, width / 2, height / 2);
				renderer->Scissor(width / 4, height / 2, width / 2, height / 2);
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
	else if (j == 1) {
		if (num_p == 1) {
			renderer->Scissor(4 * width / 5, height - width / 5, width / 5, width / 5);
			renderer->SetViewPort(4 * width / 5, height - width / 5, width / 5, width / 5);
		}
		else if (num_p == 2) {
			renderer->Scissor(4 * width / 5, height / 2 - width / 10, width / 5, width / 5);
			renderer->SetViewPort(4 * width / 5, height / 2 - width / 10, width / 5, width / 5);
		}
		else if (num_p == 3) {
			float size = width / 4.0f;
			renderer->SetViewPort(0, height / 2 + size / 8, size, size);
			renderer->Scissor(0, height / 2 + size / 8, size, size);
		}
		else if (num_p == 4) {
			renderer->Scissor(9 * width / 10, height / 2 - width / 20, width / 10, width / 10);
			renderer->SetViewPort(9 * width / 10, height / 2 - width / 20, width / 10, width / 10);
		}
	}
	else {
		if (num_p == 1) {
			renderer->Scissor(0, height - width / 5, width / 5, width / 5);
			renderer->SetViewPort(0, height - width / 5, width / 5, width / 5);
		}
		else if (num_p == 2) {
			renderer->Scissor(0, height / 2 - width / 10, width / 5, width / 5);
			renderer->SetViewPort(0, height / 2 - width / 10, width / 5, width / 5);
		}
		else if (num_p == 3) {
			float size = width / 4.0f;
			renderer->SetViewPort(3 * width / 4, height / 2 + size / 8, size, size);
			renderer->Scissor(3 * width / 4, height / 2 + size / 8, size, size);
		}
		else if (num_p == 4) {
			renderer->Scissor(0, height / 2 - width / 20, width / 10, width / 10);
			renderer->SetViewPort(0, height / 2 - width / 20, width / 10, width / 10);
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

void GraphicsPipeline::FillPaint(float dt) {
	TextureBase* depth = NULL;
	PaintBuffer->Activate();
	for (RenderNodeBase* node : allNodes) {
		node->Update(0.0f);
		if ((*node->GetChildIteratorStart())->HasTag(Tags::TGround)) {
			ground = (*node->GetChildIteratorStart());
		}
		else if ((*node->GetChildIteratorStart())->HasTag(Tags::TPaintable)) {
			(*node->GetChildIteratorStart())->SetPaintPercentage((*node->GetChildIteratorStart())->GetPaintPercentage()+0.1f);
			if ((*node->GetChildIteratorStart())->GetPaintPercentage() >= 100) {
				(*node->GetChildIteratorStart())->SetPaintPercentage(100);
				(*node->GetChildIteratorStart())->SetBeingPainted(false);
			}
			else {
				if ((*node->GetChildIteratorStart())->GetBeingPainted()) {
					shaderPaint->Activate();
					renderer->SetViewPort(1024, 1024);
					PaintBuffer->ChangeColourAttachment((*node->GetChildIteratorStart())->GetMesh()->GetTexture(1));
					paintQuad->ReplaceTexture((*node->GetChildIteratorStart())->GetMesh()->GetTexture(1),0);
					shaderPaint->SetUniform("radius_perc", (*node->GetChildIteratorStart())->GetPaintPercentage());
					shaderPaint->SetUniform("playerColor", (*node->GetChildIteratorStart())->GetColourFromPlayer());
					paintQuad->Draw();
				}
			}
		}
	}
}

void GraphicsPipeline::ChangeScene() {
	fullscreenQuad->ReplaceTexture(ResourceManager::Instance()->getTexture("loading_tex"), 0);
	cameras.clear();
	fullscreenQuad->Draw();
	NCLDebug::_ClearDebugLists();
	//NCLDebug::_ReleaseShaders();
	renderer->BindScreenFramebuffer();
	renderer->Clear(Renderer::COLOUR_DEPTH);
	renderer->SwapBuffers();
	renderer->Clear(Renderer::COLOUR_DEPTH);
}

void GraphicsPipeline::clearGraphicsPipeline() {
	allNodes.clear();
	paintableObjects.clear();
	renderlistOpaque.clear();
	renderlistTransparent.clear();
	cameras.clear();
	//viewMatrices.clear();
	//projViewMatrices.clear();
	//ResourceManager::Instance()->ReplaceTexture("gr_tex", Texture::COLOUR, 2048, 2048);
	TrailBuffer->Activate();
	renderer->Clear(Renderer::COLOUR_DEPTH);
}


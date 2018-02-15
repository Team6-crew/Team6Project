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
#include <nclgl\Graphics\Renderer\TextureFactory.h>
#include <nclgl\Graphics\FrameBufferBase.h>
#include <nclgl\Graphics\Renderer\FrameBufferFactory.h>
#include "Player.h"
#include "SceneManager.h"
#include <nclgl\GameLogic.h>

using namespace nclgl::Maths;

GraphicsPipeline::GraphicsPipeline()
	: camera(new Camera())
	, isVsyncEnabled(false)
	, screenTexWidth(0)
	, screenTexHeight(0)
	, screenFBO(NULL)
	, screenTexColor(NULL)
	, screenTexDepth(NULL)
	, shaderPresentToWindow(NULL)
	, shaderShadow(NULL)
	, shaderForwardLighting(NULL)
	, fullscreenQuad(NULL)
	, shadowFBO(NULL)
	, shadowTex(NULL)
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
	
	glGenFramebuffers(1, &TrailBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, TrailBuffer);

	glGenTextures(1, &gr_tex);
	glBindTexture(GL_TEXTURE_2D, gr_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, gr_tex, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "error";
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	minimap->SetTexture(gr_tex);
	Resize(renderer->GetWidth(), renderer->GetHeight());
}

GraphicsPipeline::~GraphicsPipeline()
{  
	SAFE_DELETE(camera);
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
		screenTexColor = TextureFactory::Instance()->MakeTexture(Texture::COLOUR, screenTexWidth, screenTexHeight);
		//Depth Texture
		screenTexDepth = TextureFactory::Instance()->MakeTexture(Texture::DEPTH, screenTexWidth, screenTexHeight);
		//Generate our Framebuffer
		screenFBO = FrameBufferFactory::Instance()->MakeFramebuffer(screenTexColor, screenTexDepth);
	}

	//Construct our Shadow Maps and Shadow UBO
	shadowTex = TextureFactory::Instance()->MakeTexture(Texture::DEPTH_ARRAY, SHADOWMAP_SIZE, SHADOWMAP_NUM);
	shadowFBO = FrameBufferFactory::Instance()->MakeFramebuffer(shadowTex);

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



void GraphicsPipeline::RenderScene()
{
	for (int i = 0; i < cameras.size(); i++) {
		camera = cameras[i];
		projViewMatrix = projViewMatrices[i];

		//Build World Transforms
		// - Most scene objects will probably end up being static, so we really should only be updating
		//   modelMatrices for objects (and their children) who have actually moved since last frame
		RenderNodeBase * ground = NULL;
		for (RenderNodeBase* node : allNodes) {
			node->Update(0.0f); //Not sure what the msec is here is for, apologies if this breaks anything in your framework!
			if ((*node->GetChildIteratorStart())->HasTag(Tags::TGround)) {
				ground = (*node->GetChildIteratorStart());
			}
		}


		GameLogic::Instance()->calculatePaintPercentage();

		SceneManager::Instance()->GetCurrentScene()->Score = GameLogic::Instance()->getPaintPerc();


		glBindFramebuffer(GL_FRAMEBUFFER, TrailBuffer);
		glViewport(0, 0, 2048, 2048);

		glUseProgram(shaderTrail->GetProgram());
		glUniform1i(glGetUniformLocation(shaderTrail->GetProgram(), "num_players"), GameLogic::Instance()->getNumPlayers());
		for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); i++) {
			std::string arr = "players[" + std::to_string(i) + "].";
			float pos_x = GameLogic::Instance()->getPlayer(i)->getRelativePosition().x;
			float pos_z = GameLogic::Instance()->getPlayer(i)->getRelativePosition().z;
			float rad = GameLogic::Instance()->getPlayer(i)->getRadius();
			Vector4 temp_col = (*GameLogic::Instance()->getPlayer(i)->Render()->GetChildIteratorStart())->GetColour();
			Vector3 trailColor = Vector3(temp_col.x, temp_col.y, temp_col.z);
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_C)) {
				trailColor = Vector3(0.0f, 1.0f, 0.0f);
			}
			glUniform1f(glGetUniformLocation(shaderTrail->GetProgram(), (arr + "pos_x").c_str()), pos_x);
			glUniform1f(glGetUniformLocation(shaderTrail->GetProgram(), (arr + "pos_z").c_str()), pos_z);
			glUniform1f(glGetUniformLocation(shaderTrail->GetProgram(), (arr + "rad").c_str()), rad);
			glUniform3fv(glGetUniformLocation(shaderTrail->GetProgram(), (arr + "trailColor").c_str()), 1, (float*)&trailColor);

		}


		trailQuad->Draw();
		ground->GetMesh()->SetTexture(gr_tex);


		//Build Transparent/Opaque Renderlists
		BuildAndSortRenderLists();
	//Build World Transforms
	// - Most scene objects will probably end up being static, so we really should only be updating
	//   modelMatrices for objects (and their children) who have actually moved since last frame
	for (RenderNodeBase* node : allNodes)
		node->Update(0.0f); //Not sure what the msec is here is for, apologies if this breaks anything in your framework!
	
	//Build Transparent/Opaque Renderlists
	BuildAndSortRenderLists();

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
			[&](RenderNode* node)
		{
			glUniformMatrix4fv(uModelMtx, 1, GL_FALSE, (float*)&node->GetWorldTransform());
		}
			[&](RenderNodeBase* node)
			{
				shaderShadow->SetUniform("uModelMtx", node->GetWorldTransform());
			}
		);




		//Render scene to screen fbo
		glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
		glViewport(0, 0, screenTexWidth, screenTexHeight);
		glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderForwardLighting->GetProgram());
		glUniformMatrix4fv(glGetUniformLocation(shaderForwardLighting->GetProgram(), "uProjViewMtx"), 1, GL_FALSE, (float*)&projViewMatrix);
		glUniform1i(glGetUniformLocation(shaderForwardLighting->GetProgram(), "uDiffuseTex"), 0);
		glUniform3fv(glGetUniformLocation(shaderForwardLighting->GetProgram(), "uCameraPos"), 1, (float*)&camera->GetPosition());
		glUniform3fv(glGetUniformLocation(shaderForwardLighting->GetProgram(), "uAmbientColor"), 1, (float*)&ambientColor);
		glUniform3fv(glGetUniformLocation(shaderForwardLighting->GetProgram(), "uLightDirection"), 1, (float*)&lightDirection);
		glUniform1fv(glGetUniformLocation(shaderForwardLighting->GetProgram(), "uSpecularFactor"), 1, &specularFactor);

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
		shaderForwardLighting->SetUniform("uShadowTransform[0]", SHADOWMAP_NUM, shadowProjView);
		shaderForwardLighting->SetUniform("uShadowTex", 2);
		shaderForwardLighting->SetUniform("uShadowSinglePixel", Vector2(1.f / SHADOWMAP_SIZE, 1.f / SHADOWMAP_SIZE));

		shadowTex->Bind(2);

		RenderAllObjects(false,
			[&](RenderNode* node)
		{
			glUniformMatrix4fv(uModelMtx, 1, GL_FALSE, (float*)&node->GetWorldTransform());
			glUniform4fv(uColor, 1, (float*)&node->GetColor());
		}
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
		ScreenPicker::Instance()->RenderPickingScene(projViewMatrix, Matrix4::Inverse(projViewMatrix), screenTexDepth->TempGetID(), screenTexWidth, screenTexHeight);

		screenFBO->Activate();
		renderer->SetViewPort(screenTexWidth, screenTexHeight);
		//NCLDEBUG - World Debug Data (anti-aliased)		
		NCLDebug::_RenderDebugDepthTested();
		NCLDebug::_RenderDebugNonDepthTested();



		//Downsample and present to screen

		
		
		for (int j = 0; j < 2; j++) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glEnable(GL_SCISSOR_TEST);
			AdjustViewport(i,j);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			glDisable(GL_SCISSOR_TEST);
	//Downsample and present to screen
		renderer->BindScreenFramebuffer();
		renderer->SetViewPort(renderer->GetWidth(), renderer->GetHeight());
		renderer->Clear(Renderer::COLOUR_DEPTH);

			float superSamples = (float)(numSuperSamples);
			glUseProgram(shaderPresentToWindow->GetProgram());
			glUniform1i(glGetUniformLocation(shaderPresentToWindow->GetProgram(), "uColorTex"), 0);
			glUniform1f(glGetUniformLocation(shaderPresentToWindow->GetProgram(), "uGammaCorrection"), gammaCorrection);
			glUniform1f(glGetUniformLocation(shaderPresentToWindow->GetProgram(), "uNumSuperSamples"), superSamples);
			glUniform2f(glGetUniformLocation(shaderPresentToWindow->GetProgram(), "uSinglepixel"), 1.f / screenTexWidth, 1.f / screenTexHeight);

			fullscreenQuad->SetTexture(screenTexColor);

			if (j == 0) {

				fullscreenQuad->Draw();
			}
			else {
				tempProj = projMatrix;
				tempView = viewMatrix;
				projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
				viewMatrix.ToIdentity();
				minimap->Draw();
				projMatrix = tempProj;
				viewMatrix = tempView;
			}
		float superSamples = (float)(numSuperSamples);
		shaderPresentToWindow->Activate();
		shaderPresentToWindow->SetUniform("uColorTex", 0);
		shaderPresentToWindow->SetUniform("uGammaCorrection", gammaCorrection);
		shaderPresentToWindow->SetUniform("uNumSuperSamples", superSamples);
		shaderPresentToWindow->SetUniform("uSinglepixel", Vector2(1.f / screenTexWidth, 1.f / screenTexHeight));
		fullscreenQuad->SetTexture(screenTexColor);
		fullscreenQuad->Draw();

			//NCLDEBUG - Text Elements (aliased)
			NCLDebug::_RenderDebugClipSpace();
			NCLDebug::_ClearDebugLists();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	OGLRenderer::SwapBuffers();
		//NCLDEBUG - Text Elements (aliased)
		NCLDebug::_RenderDebugClipSpace();
		NCLDebug::_ClearDebugLists();
	
		renderer->SwapBuffers();
}

void GraphicsPipeline::AdjustViewport(int i, int j) {
	int num_p = GameLogic::Instance()->getNumPlayers();
	if (j == 0) {
		if (num_p == 1) {
			glViewport(0, 0, width, height);
		}
		else if (num_p == 2) {
			projMatrix = Matrix4::Perspective(PROJ_NEAR, PROJ_FAR, ((float)width) / ((float)height / 2.0f), PROJ_FOV);
			if (i == 0) {
				glViewport(0, height / 2, width, height / 2);
				glScissor(0, height / 2, width, height / 2);
			}
			else {
				glViewport(0, 0, width, height / 2);
				glScissor(0, 0, width, height / 2);
			}
		}
		else if (num_p == 3) {
			if (i == 0) {
				glViewport(0, height / 2, width / 2, height / 2);
				glScissor(0, height / 2, width / 2, height / 2);
			}
			else if (i == 1) {
				glViewport(0, 0, width / 2, height / 2);
				glScissor(0, 0, width / 2, height / 2);
			}
			else {
				glViewport(width / 2, 0, width / 2, height / 2);
				glScissor(width / 2, 0, width / 2, height / 2);
			}
		}
		else if (num_p == 4) {
			if (i == 0) {
				glViewport(0, height / 2, width / 2, height / 2);
				glScissor(0, height / 2, width / 2, height / 2);
			}
			else if (i == 1) {
				glViewport(width / 2, height / 2, width / 2, height / 2);
				glScissor(width / 2, height / 2, width / 2, height / 2);
			}
			else if (i == 2) {
				glViewport(0, 0, width / 2, height / 2);
				glScissor(0, 0, width / 2, height / 2);
			}
			else {
				glViewport(width / 2, 0, width / 2, height / 2);
				glScissor(width / 2, 0, width / 2, height / 2);
			}
		}
	}
	else {
		if (num_p == 1) {
			glScissor(4 * width / 5, 0, width / 5, width / 5);
			glViewport(4 * width / 5, 0, width / 5, width / 5);
		}
		else if (num_p == 2) {
			glScissor(4 * width / 5, height/2 - width/10, width / 5, width / 5);
			glViewport(4 * width / 5, height / 2 - width / 10, width / 5, width / 5);
		}
		else if (num_p == 3) {
			glViewport(width / 2, height / 2, width / 2, height / 2);
			glScissor(width / 2, height / 2, width / 2, height / 2);
		}
		else if (num_p == 4) {
			glScissor(3 * width / 7, height / 2 - width / 14, width / 7, width / 7);
			glViewport(3 * width / 7, height / 2 - width / 14, width / 7, width / 7);
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
	projViewMatrices.push_back(projMatrix);
	cam->SetPitch(-20.0f);
	return cam;
}
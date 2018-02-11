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
{
	renderer = RenderFactory::Instance()->MakeRenderer();

	LoadShaders();
	NCLDebug::_LoadShaders();

	fullscreenQuad = OGLMesh::GenerateQuad();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	sceneBoundingRadius = 30.f; ///Approx based on scene contents

	camera->SetPosition(Vector3(0.0f, 10.0f, 15.0f));
	camera->SetYaw(0.f);
	camera->SetPitch(-20.f);
	InitializeDefaults();
	Resize(renderer->GetWidth(), renderer->GetHeight());
}

GraphicsPipeline::~GraphicsPipeline()
{
	SAFE_DELETE(camera);
	
	SAFE_DELETE(fullscreenQuad);

	SAFE_DELETE(shaderPresentToWindow);
	SAFE_DELETE(shaderShadow);
	SAFE_DELETE(shaderForwardLighting);

	NCLDebug::_ReleaseShaders();

	if (screenFBO)
	{
		glDeleteFramebuffers(1, &screenFBO);
		screenFBO = NULL;
	}

	if (shadowFBO)
	{
		glDeleteFramebuffers(1, &shadowFBO);
		shadowFBO = NULL;
	}
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
	GLuint status;

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
		if (!screenFBO) glGenFramebuffers(1, &screenFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexColor->TempGetID(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, screenTexDepth->TempGetID(), 0);

		GLenum buf = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &buf);
		//Validate our framebuffer
		if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
		{
			NCLERROR("Unable to create Screen Framebuffer! StatusCode: %x", status);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//Construct our Shadow Maps and Shadow UBO
	shadowTex = TextureFactory::Instance()->MakeTexture(Texture::DEPTH_ARRAY, SHADOWMAP_SIZE, SHADOWMAP_NUM);

	if (!shadowFBO) glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex->TempGetID(), 0);
	glDrawBuffers(0, GL_NONE);
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
	{
		NCLERROR("Unable to create Shadow Framebuffer! StatusCode: %x", status);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//m_ShadowUBO._ShadowMapTex = glGetTextureHandleARB(m_ShadowTex);
	//glMakeTextureHandleResidentARB(m_ShadowUBO._ShadowMapTex);
}


void GraphicsPipeline::UpdateScene(float dt)
{
	if (!ScreenPicker::Instance()->HandleMouseClicks(dt))
		camera->HandleMouse(dt);

	camera->HandleKeyboard(dt);
	renderer->SetViewMatrix(camera->BuildViewMatrix());
	projViewMatrix = renderer->GetProjMatrix() * renderer->GetViewMatrix();

	NCLDebug::_SetDebugDrawData(
		renderer->GetProjMatrix(),
		renderer->GetViewMatrix(),
		camera->GetPosition());
}

void GraphicsPipeline::RenderScene()
{
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
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex->TempGetID(), 0);
		glViewport(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);
		glClear(GL_DEPTH_BUFFER_BIT);

		shaderShadow->Activate();
		shaderShadow->SetUniform("uShadowTransform[0]", SHADOWMAP_NUM, shadowProjView);

		RenderAllObjects(true,
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
	

		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D_ARRAY, shadowTex->TempGetID());

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
		ScreenPicker::Instance()->RenderPickingScene(projViewMatrix, Matrix4::Inverse(projViewMatrix), screenTexDepth->TempGetID(), screenTexWidth, screenTexHeight);

		glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
		glViewport(0, 0, screenTexWidth, screenTexHeight);
		//NCLDEBUG - World Debug Data (anti-aliased)		
		NCLDebug::_RenderDebugDepthTested();
		NCLDebug::_RenderDebugNonDepthTested();
	


	//Downsample and present to screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, renderer->GetWidth(), renderer->GetHeight());
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

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
	

		renderer->SwapBuffers();
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
			glCullFace(GL_FRONT);
			node.first->Draw();

			glCullFace(GL_BACK);
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
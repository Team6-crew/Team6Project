#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\OcTree.h>
#include "TextMesh.h"
#include "../nclgl/OGLRenderer.h"
#include "RendererUI.h"

//Fully striped back scene to use as a template for new scenes.
class MainMenu : public Scene
{
public:
	RenderNode * cam;
	RenderNode * text;
	GraphicsPipeline* gp;
	Shader* currentShader;
	Font*	basicFont;	//A font! a basic one...
	MainMenu(const std::string& friendly_name, GraphicsPipeline * GP)
		: Scene(friendly_name)
	{
		gp = GP;
	}

	virtual ~MainMenu()
	{
	}

	virtual void OnInitializeScene() override
	{
		Scene::OnInitializeScene();
		currentShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

		//if (!currentShader->LinkProgram()) {
		//	return;
		//}
		/*
		Just makes a new 'font', a struct containing a texture (of the tahoma font)
		and how many characters across each axis the font contains. (look at the
		font texture in paint.net if you don't quite 'get' this)
		*/
		basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

		//The font is not alpha blended! It has a black background.
		//but that doesn't matter, we can fiddle blend func to do 
		//'additive blending', meaning black won't show up ;)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		//init = true;
		cam = new RenderNode();
		text = new RenderNode();
		
		cam->SetTransform(Matrix4::Translation(Vector3(0, 10, 25)));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer

		glUseProgram(currentShader->GetProgram());	//Enable the shader...
													//And turn on texture unit 0
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

		//Render function to encapsulate our font rendering!
		gp->drawtex("This is orthographic text!", Vector3(0, 0, 0), 16.0f, false);
		//drawtex("This is perspective text!!!!", Vector3(0, 0, -1000), 64.0f, true);

		glUseProgram(0);	//That's everything!

		//SwapBuffers();
	}


	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);
		Camera * camera = GraphicsPipeline::Instance()->GetCamera();

		float yaw = camera->GetYaw();
		float pitch = camera->GetPitch();

		float rotation = 0.0f;

		float power = 15.0f;

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_I))
		{
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_K))
		{
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_J))
		{
			rotation = 0.5f;
			camera->SetYaw(yaw + rotation);
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_L))
		{
			rotation = -0.5f;
			camera->SetYaw(yaw + rotation);
		}

		camera->SetPosition(cam->GetWorldTransform().GetPositionVector());
	}

	//void GraphicsPipeline::Instance() drawText(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
	//	//Create a new temporary TextMesh, using our line of text and our font
	//	TextMesh* mesh = new TextMesh(text, *basicFont);

	//	//This just does simple matrix setup to render in either perspective or
	//	//orthographic mode, there's nothing here that's particularly tricky.
	//	if (perspective) {
	//		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
	//		viewMatrix = camera->BuildViewMatrix();
	//		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	//	}
	//	else {
	//		//In ortho mode, we subtract the y from the height, so that a height of 0
	//		//is at the top left of the screen, which is more intuitive
	//		//(for me anyway...)
	//		modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
	//		viewMatrix.ToIdentity();
	//		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	//	}
	//	//Either way, we update the matrices, and draw the mesh
	//	UpdateShaderMatrices();
	//	mesh->Draw();

	//	delete mesh; //Once it's drawn, we don't need it anymore!
	//}
};




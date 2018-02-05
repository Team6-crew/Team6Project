#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "TextMesh.h"

class RendererUI : public OGLRenderer {
public:
	RendererUI(Window &parent);
	virtual ~RendererUI(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	/*
	Draws the passed in line of text, at the passed in position. Can have an optional font size, and
	whether to draw it in orthographic or perspective mode.
	*/

	void drawtex(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);

protected:
	Camera * camera;
	Font*	basicFont;	//A font! a basic one...
};


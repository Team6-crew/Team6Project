#pragma once

//Number of cascading shadow maps 
// - As we don't have the ability to set shadow defines, any changes here also need to be
//   mirrored inside "TechFragForwardRender.glsl" and "TechGeomShadow.glsl" shaders.
#define SHADOWMAP_NUM 4			

//Size of the shadows maps in pixels
// - With a size of 4096x4096 (and 4 shadowmaps) using 32bit floats this
//   currently results in shadows using up 256MB of space. Which is quite alot,
//   but also currently the only potentially memory sensitive thing we do in this 
//   renderer so it's fine.
#define SHADOWMAP_SIZE 4096


#define PROJ_FAR      2000.0f			//Can see for 50m - setting this too far really hurts shadow quality as they attempt to cover the entirety of the view frustum
#define PROJ_NEAR     0.1f			//Nearest object @ 10cm
#define PROJ_FOV      45.0f			//45 degree field of view

namespace RenderConstants
{
	extern unsigned int screenWidth;
	extern unsigned int screenHeight;
	extern bool			fullScreen;

	extern const char*  windowTitle;
}
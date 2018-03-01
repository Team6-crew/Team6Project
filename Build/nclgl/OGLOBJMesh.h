/******************************************************************************
Class:OBJMesh
Implements:Mesh, MD5MeshInstance
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description: Implementation of the Wavefront OBJ mesh format.

This was going to be a full tutorial, and then I implemented it and was
reminded of how horrific OBJ files can be, so just dumped the code in the
framework, instead.

You'll very quickly find OBJ meshes that can't be loaded by this loader.

First up - any mesh with anything other than triangles in it won't work. Quads
should be trivial to add in, n-sided polygons should be OK if they are convex
(add a point in the middle and create triangles out of pairs of points).

Some OBJ meshes use negative indices, which won't work.

In both cases, loading the OBJ into Blender or maybe Milkshape, and exporting
them out as OBJs again might create a file more likely to load.

OBJ files are ok for simple geometry (the tutorial series uses them for 2
slightly different cubes, and an icosphere), but generally don't work very
well for 'big' geometry.

The simple loader doesn't do anything fancy with indices - have fun trying
to get that working!

The 'Stanford Bunny' OBJ does load up with this though, if you really want
to see a rabbit.

***
** **
**   **
**   **         ****
**   **       **   ****
**  **       *   **   **
**  *      *  **  ***  **
**  *    *  **     **  *
** **  ** **        **
**   **  **
*           *
*             *
*    0     0    *
*   /   @   \   *
*   \__/ \__/   *
*     W     *
**     **
*****
-_-_-_-_-_-_-_,------,
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""

*//////////////////////////////////////////////////////////////////////////////

/*
As this tutorial series progresses, you'll learn how to generate normals, tangents,
and how to use bumpmaps. In order for this class to compile before these features
are introduced, 'advanced' functionality has been disabled using the preprocessor.

If you want to play around with OBJMeshes in the first real time lighting tutorial,
uncomment the OBJ_USE_NORMALS define. If you want to use an OBJMesh in the second real
time lighting tutorial, uncomment both OBJ_USE_NORMALS and OBJ_USE_TANGENTS_BUMPMAPS
*/

#define OBJ_USE_NORMALS
#define OBJ_USE_TANGENTS_BUMPMAPS

//New! 

#define OBJ_FIX_TEXTURES

#define WIN_OGL
#ifdef WIN_OGL
#pragma once
#include "OBJMeshBase.h"
#include <nclgl\Graphics\Renderer\OpenGL\OGLMesh.h>
/*
OBJSubMesh structs are used to temporarily keep the data loaded
in from the OBJ files, before being parsed into a series of
Meshes
*/

class OGLOBJMesh : public OGLMesh, public OBJMeshBase {
public:
	OGLOBJMesh(void) {};
	OGLOBJMesh(std::string filename) { LoadOBJMesh(filename); };
	~OGLOBJMesh(void) {};
	bool	LoadOBJMesh(std::string filename);

	virtual void Draw();

	virtual void SetTexture(TextureBase* t) {
		texture = (OGLTexture*)t;
	}
protected:
	void	SetTexturesFromMTL(std::string &mtlFile, std::string &mtlType);

	void	FixTextures(MTLInfo &info);

};

#endif
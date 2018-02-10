/******************************************************************************
Class: CommonMeshes
Implements:
Author: 
	Pieran Marris <p.marris@newcastle.ac.uk>
Description:
	A quick and dirty library of common meshes, saves loading the same common meshes (cube, sphere etc) over and over again.

	These are loaded when the GraphicsPipeline is first initialised and released when it is deleted, so can
	be globally accessed for the entirity of any Scene/GameObject.

*//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <nclgl\Graphics\MeshBase.h>
#include "../ExternalLibs/GLEW/include/GL/glew.h"

class Scene;

class CommonMeshes
{
	friend class SceneManager; //Initializes/Destroys the given meshes within it's own lifecycle

public:
	//To use these resources, just make a copy of the rendernode structure as required
	// e.g RenderNode* cube_copy = new RenderNode(*CommonMeshes::Cube());

	//Cube
	static MeshBase* Cube()			{ return m_pCube; }

	//Sphere
	static MeshBase* Sphere()		{ return m_pSphere; }



	//PhysicsEngine Checkerboard - Hidden here for reasons of laziness
	static const GLuint CheckerboardTex()   { return m_pCheckerboardTex; }


protected:
	//Called by SceneRenderer
	static void InitializeMeshes();
	static void ReleaseMeshes();

protected:
	static MeshBase* m_pCube;
	static MeshBase* m_pSphere;

	static GLuint m_pCheckerboardTex;
};
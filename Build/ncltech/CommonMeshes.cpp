#include "CommonMeshes.h"
#include <nclgl\NCLDebug.h>
#include <nclgl\OBJMesh.h>
#include "../ExternalLibs/SOIL/include/SOIL.h" 
#include <nclgl\Graphics\TextureBase.h>
#include <nclgl\Graphics\Renderer\TextureFactory.h>

MeshBase*	  CommonMeshes::m_pCube		= NULL;
MeshBase*	  CommonMeshes::m_pSphere	= NULL;

TextureBase*  CommonMeshes::m_pCheckerboardTex = NULL;

void CommonMeshes::InitializeMeshes()
{
	if (m_pCube == NULL)
	{

		//m_pCheckerboardTex = TextureFactory::Instance()->MakeTexture(TEXTUREDIR"checkerboard.tga");
		m_pCheckerboardTex = TextureFactory::Instance()->MakeTexture(TEXTUREDIR"Blank.tga");
		m_pCheckerboardTex->SetTextureFiltering(true);
		m_pCheckerboardTex->SetTextureWrapping(true);

		m_pCube = new OBJMesh(MESHDIR"cube.obj");
		m_pCube->SetTexture(m_pCheckerboardTex);

		m_pSphere = new OBJMesh(MESHDIR"sphere.obj");
		m_pSphere->SetTexture(m_pCheckerboardTex);
	}
}

void CommonMeshes::ReleaseMeshes()
{
	SAFE_DELETE(m_pCube);
	SAFE_DELETE(m_pSphere);
	m_pCheckerboardTex = nullptr;
}
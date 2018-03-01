#include "CommonMeshes.h"
#include <nclgl\NCLDebug.h>
#include <nclgl\OBJMeshBase.h>
#include "../ExternalLibs/SOIL/include/SOIL.h" 
#include <nclgl\Graphics\TextureBase.h>
#include <nclgl\Graphics\Renderer\TextureFactory.h>
#include "../nclgl/MeshFactory.h"

OBJMeshBase*	  CommonMeshes::m_pCube		= NULL;
OBJMeshBase*	  CommonMeshes::m_pSphere	= NULL;

TextureBase*  CommonMeshes::m_pCheckerboardTex = NULL;

void CommonMeshes::InitializeMeshes()
{
	if (m_pCube == NULL)
	{

		m_pCheckerboardTex = TextureFactory::Instance()->MakeTexture(std::string(TEXTUREDIR"checkerboard"));
		m_pCheckerboardTex->SetTextureFiltering(true);
		m_pCheckerboardTex->SetTextureWrapping(true);

		m_pCube = MeshFactory::Instance()->MakeOBJMesh(MESHDIR"cube.obj");
		m_pCube->SetTexture(m_pCheckerboardTex);

		m_pSphere = MeshFactory::Instance()->MakeOBJMesh(MESHDIR"sphere.obj");
		m_pSphere->SetTexture(m_pCheckerboardTex);
	}
}

void CommonMeshes::ReleaseMeshes()
{
	SAFE_DELETE(m_pCube);
	SAFE_DELETE(m_pSphere);
	m_pCheckerboardTex = nullptr;
}
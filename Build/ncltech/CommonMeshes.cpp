#include "CommonMeshes.h"
#include <nclgl\NCLDebug.h>
#include <nclgl\OBJMesh.h>
#include "../ExternalLibs/SOIL/include/SOIL.h" 
#include <nclgl\Graphics\TextureBase.h>
#include <nclgl\Graphics\Renderer\TextureFactory.h>
#include <nclgl\ResourceManager.h>

MeshBase*	  CommonMeshes::m_pCube		= NULL;
MeshBase*	  CommonMeshes::m_pSphere	= NULL;


MeshBase * CommonMeshes::NewCube() {
	TextureBase* m_pCheckerboardTex = NULL;
	TextureBase* m_secondTex = NULL;
	m_pCheckerboardTex = ResourceManager::Instance()->getTexture(TEXTUREDIR"Blank.tga");
	m_pCheckerboardTex->SetTextureFiltering(true);
	m_pCheckerboardTex->SetTextureWrapping(true);
	m_secondTex = ResourceManager::Instance()->getTexture(TEXTUREDIR"transparent.png");
	m_secondTex->SetTextureFiltering(true);
	m_secondTex->SetTextureWrapping(true);
	MeshBase * cube = new OBJMesh(MESHDIR"cube.obj");
	cube->SetTexture(m_pCheckerboardTex);
	cube->SetTexture(m_secondTex);
	return cube;
}
MeshBase * CommonMeshes::NewPaintableCube() {
	TextureBase* m_pCheckerboardTex = NULL;
	TextureBase* m_secondTex = NULL;
	m_pCheckerboardTex = ResourceManager::Instance()->getTexture(TEXTUREDIR"checkerboard.tga");
	m_pCheckerboardTex->SetTextureFiltering(true);
	m_pCheckerboardTex->SetTextureWrapping(true);
	m_secondTex = ResourceManager::Instance()->getTexture(TEXTUREDIR"transparent.png");
	m_secondTex->SetTextureFiltering(true);
	m_secondTex->SetTextureWrapping(true);
	MeshBase * cube = new OBJMesh(MESHDIR"cube_Paintable.obj");
	cube->SetTexture(m_pCheckerboardTex);
	cube->SetTexture(m_secondTex);
	return cube;
}


MeshBase * CommonMeshes::NewSphere() {
	TextureBase* m_pCheckerboardTex = NULL;
	TextureBase* m_secondTex = NULL;
	m_pCheckerboardTex = ResourceManager::Instance()->getTexture(TEXTUREDIR"checkerboard.tga");
	m_pCheckerboardTex->SetTextureFiltering(true);
	m_pCheckerboardTex->SetTextureWrapping(true);
	m_secondTex = ResourceManager::Instance()->getTexture(TEXTUREDIR"transparent.png");
	m_secondTex->SetTextureFiltering(true);
	m_secondTex->SetTextureWrapping(true);
	MeshBase * cube = new OBJMesh(MESHDIR"sphere.obj");
	cube->SetTexture(m_pCheckerboardTex);
	cube->SetTexture(m_secondTex);
	return cube;
}

void CommonMeshes::ReleaseMeshes()
{
	SAFE_DELETE(m_pCube);
	SAFE_DELETE(m_pSphere);
}

/******************************************************************************
Class:Mesh
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:Wrapper around OpenGL primitives, geometry and related 
OGL functions.

There's a couple of extra functions in here that you didn't get in the tutorial
series, to draw debug normals and tangents. 


-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////
#ifdef WIN_OGL
#pragma once

#include <nclgl/Graphics/Renderer/OpenGL/OGLRenderer.h>
#include <nclgl\Graphics\MeshBase.h>
#include <nclgl\Graphics\Renderer\OpenGL\OGLTexture.h>
#include <vector>



#include <GL/glew.h>
//A handy enumerator, to determine which member of the bufferObject array
//holds which data
enum MeshBuffer {
	VERTEX_BUFFER	,
	COLOUR_BUFFER	, 
	TEXTURE_BUFFER	,
	NORMAL_BUFFER	, 
	TANGENT_BUFFER	,
	INDEX_BUFFER	,
	MAX_BUFFER
};

namespace nclgl
{
	namespace Maths
	{
		class Vector2;
		class Vector3;
		class Vector4;
	}
}


class OGLMesh	: public MeshBase {
public:
	friend class MD5Mesh;
	OGLMesh(void);
	OGLMesh(const OGLMesh& rhs);
	virtual ~OGLMesh(void);

	void Draw() override;

	//Generates a single triangle, with RGB colours
	static OGLMesh*	GenerateTriangle();

	static OGLMesh*	GenerateCone(float segments);
	static OGLMesh*	TestTriangle(float texRotation);
	static OGLMesh*	TestQuad(float texRotation);

	//Generates a single white quad, going from -1 to 1 on the x and z axis.
	static OGLMesh*	GenerateQuad();
	static OGLMesh*	GenerateQuadAlt();

	//Sets the Mesh's diffuse map. Takes an OpenGL texture 'name'
	void	SetTexture(TextureBase* texture)	override;
	//Gets the Mesh's diffuse map. Returns an OpenGL texture 'name'
	OGLTexture*	  GetTexture()			{return texture;}

	//Sets the Mesh's bump map. Takes an OpenGL texture 'name'
	void	SetBumpMap(OGLTexture*	 tex)	{bumpTexture = tex;}
	//Gets the Mesh's bump map. Returns an OpenGL texture 'name'
	OGLTexture*	  GetBumpMap()			{return bumpTexture;}

	//Extra stuff!!!! Aren't I nice?
	void	DrawDebugNormals(float length = 5.0f);
	void	DrawDebugTangents(float length = 5.0f);


	//Buffers all VBO data into graphics memory. Required before drawing!
	void	BufferData();


	//Generates normals for all facets. Assumes geometry type is GL_TRIANGLES...
	void	GenerateNormals();

	//Generates tangents for all facets. Assumes geometry type is GL_TRIANGLES...
	void	GenerateTangents();

	//Helper function for GenerateTangents
	nclgl::Maths::Vector3 GenerateTangent(const nclgl::Maths::Vector3 &a, 
										  const nclgl::Maths::Vector3 &b, 
										  const nclgl::Maths::Vector3 &c, 
										  const nclgl::Maths::Vector2 &ta, 
										  const nclgl::Maths::Vector2 &tb, 
										  const nclgl::Maths::Vector2 &tc);

	//VAO for this mesh
	GLuint	arrayObject;
	//VBOs for this mesh
	GLuint	bufferObject[MAX_BUFFER];
	//Number of vertices for this mesh
	GLuint	numVertices;
	//Primitive type for this mesh (GL_TRIANGLES...etc)
	GLuint	type;
	//OpenGL texture name for the diffuse map
	OGLTexture*	texture;

	//Stuff introduced later on in the tutorials!!

	//Number of indices for this mesh
	GLuint			numIndices;

	//OpenGL texture name for the bump map
	OGLTexture*				bumpTexture;

	//You might wonder why we keep pointers to vertex data once
	//it's sent off to graphics memory. For basic meshes, there's no
	//real need to keep it, so feel free to delete it. For MD5Meshes,
	//we need access to the vertex data for skinning per frame...

	//Pointer to vertex position attribute data (badly named...?)
	nclgl::Maths::Vector3*		vertices;
	//Pointer to vertex colour attribute data
	nclgl::Maths::Vector4*		colours;
	//Pointer to vertex texture coordinate attribute data
	nclgl::Maths::Vector2*		textureCoords;
	//Pointer to vertex normals attribute data
	nclgl::Maths::Vector3*		normals;
	//Pointer to vertex tangents attribute data
	nclgl::Maths::Vector3*		tangents;
	//Pointer to vertex indices attribute data
	unsigned int*	indices;
};

#endif
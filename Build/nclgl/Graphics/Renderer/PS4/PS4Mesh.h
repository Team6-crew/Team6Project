#ifdef PSTATION4

#pragma once

#include "PS4MemoryAware.h"
#include "../../MeshBase.h"
#include "PS4Texture.h"

#include <nclgl\Graphics\Renderer\PS4\nclPS4Interface.h>
class TextureBase;

namespace sce
{
	namespace Vectormath
	{
		namespace Scalar
		{
			namespace Aos
			{
				class Vector2;
				class Vector3;
			}
		}
	}
}


class PS4Mesh : public PS4MemoryAware, public MeshBase
{
public:
	PS4Mesh();
	~PS4Mesh();

	void Draw();
	void SetTexture(TextureBase* texture);

	inline void SetGraphicsContext(sce::Gnmx::GnmxGfxContext* context) { currentGfxContext = context; }

	static PS4Mesh* GenerateQuad();

	void	SetBumpMap(TextureBase* text) { bumpTexture = static_cast<PS4Texture*>(text); }

protected: // GPU data
	void	BufferData();

	// Will store vertex data as an interleaved buffer on the GPU 
	struct MeshVertex
	{
		float position[3];
		float textureCoord[2];
		float normal[3];
		float tangent[3];
	};

	int*		indexBuffer;
	MeshVertex*	vertexBuffer;

	// Vertex Attributes
	int					numAttributes;
	sce::Gnm::Buffer*	attributeBuffers;

protected: // Buffer data funtions
	void AllocateBuffers();
	void CopyDataToBuffers();
	void SetupVertexAttribs();

	// Marks buffer as read only and marks buffer as containing vertex information
	void InitAttributeBuffer(sce::Gnm::Buffer &buffer, sce::Gnm::DataFormat format, void*offset);

protected: // GenerateShape Functions
	void AllocateVertexMemory();

	//Quad Functions
	void AssignQuadProperties();
	void DefineQuadVertices();
	void DefineQuadTexCoords();
	void SetupQuadVectors();

protected: // Vertex Data
	unsigned int numVertices = 0;
	unsigned int numIndices = 0;

	sce::Vectormath::Scalar::Aos::Vector3* vertices;
	sce::Vectormath::Scalar::Aos::Vector2* texCoords;
	sce::Vectormath::Scalar::Aos::Vector3* normals;
	sce::Vectormath::Scalar::Aos::Vector3* tangents;

	int*	 indices;


protected: // 
	sce::Gnmx::GnmxGfxContext* currentGfxContext = nullptr;


protected:
	// Representation of indices e.g. kIndexSize32
	sce::Gnm::IndexSize		indexType;
	// Primitive type e.g. lines, trianglestrip
	sce::Gnm::PrimitiveType primitiveType;


	PS4Texture* texture;
	PS4Texture* bumpTexture;


	void PS4Mesh::GenerateNormals() {
		if (!normals) {
			normals = new sce::Vectormath::Scalar::Aos::Vector3[numVertices];
		}
		for (int i = 0; i < numVertices; ++i) {
			normals[i] = sce::Vectormath::Scalar::Aos::Vector3();
		}
		if (indices) {
			uint i = 0;
			int test = 0;
			for (i = 0; i < numIndices; i += 3) {
				int a = indices[i];
				int b = indices[i + 1];
				int c = indices[i + 2];

				sce::Vectormath::Scalar::Aos::Vector3 normal = CrossPS4((vertices[b] - vertices[a]), (vertices[c] - vertices[a]));

				normals[a] += normal;
				normals[b] += normal;
				normals[c] += normal;

				test += 3;
			}
			bool asdf = true;
		}
		else {
			for (int i = 0; i < numVertices; i += 3) {
				sce::Vectormath::Scalar::Aos::Vector3 &a = vertices[i];
				sce::Vectormath::Scalar::Aos::Vector3 &b = vertices[i + 1];
				sce::Vectormath::Scalar::Aos::Vector3 &c = vertices[i + 2];

				sce::Vectormath::Scalar::Aos::Vector3 normal = CrossPS4(b - a, c - a);

				normals[i] = normal;
				normals[i + 1] = normal;
				normals[i + 2] = normal;
			}
			for (int i = 0; i < numVertices; ++i) {
				NormalisePS4(normals[i]);
			}
		}
	}
};


#endif
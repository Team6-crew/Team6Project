#ifdef PSTATION4


#include "PS4Mesh.h"

#include <graphics\api_gnm\toolkit\geommath\geommath.h>

PS4Mesh::PS4Mesh()
{
	indexType = sce::Gnm::IndexSize::kIndexSize32;
	primitiveType = sce::Gnm::PrimitiveType::kPrimitiveTypeTriList;
}


PS4Mesh::~PS4Mesh()
{
}

PS4Mesh* PS4Mesh::GenerateQuad()
{
	PS4Mesh* mesh = new PS4Mesh();

	// Setup Mesh
	mesh->AssignQuadProperties();
	mesh->AllocateVertexMemory();
	mesh->DefineQuadVertices();
	mesh->DefineQuadTexCoords();
	mesh->SetupQuadVectors();

	// Buffer to GPU
	mesh->BufferData();

	return mesh;
}

void	PS4Mesh::BufferData()
{
	AllocateBuffers();
	CopyDataToBuffers();
	SetupVertexAttribs();
}


void PS4Mesh::AllocateBuffers()
{
	// Calculate amount of memory to allocate on the GPU
	uint vertexBufferSize = numVertices * sizeof(MeshVertex);
	uint indexBufferSize = numIndices * sizeof(int);

	indexBuffer = static_cast<int*> (garlicAllocator.allocate(
		indexBufferSize, sce::Gnm::kAlignmentOfBufferInBytes));

	vertexBuffer = static_cast<MeshVertex*>(garlicAllocator.allocate(
		vertexBufferSize, sce::Gnm::kAlignmentOfBufferInBytes));

	// Register for debugging
	sce::Gnm::registerResource(nullptr, ownerHandle, indexBuffer, indexBufferSize,
		"IndexData", sce::Gnm::kResourceTypeIndexBufferBaseAddress, 0);
	sce::Gnm::registerResource(nullptr, ownerHandle, vertexBuffer, vertexBufferSize,
		"VertexData", sce::Gnm::kResourceTypeIndexBufferBaseAddress, 0);
}

void PS4Mesh::CopyDataToBuffers()
{
	// Copy the vertex arrays into interleaved buffer
	for (int i = 0; i < numVertices; ++i)
	{
		memcpy(&vertexBuffer[i].position, &vertices[i], sizeof(float) * 3);
		memcpy(&vertexBuffer[i].textureCoord, &texCoords[i], sizeof(float) * 2);
		memcpy(&vertexBuffer[i].normal, &normals[i], sizeof(float) * 3);
		memcpy(&vertexBuffer[i].tangent, &tangents[i], sizeof(float) * 3);
	}

	for (int i = 0; i < numIndices; ++i)
	{
		indexBuffer[i] = indices[i];
	}
}

void PS4Mesh::SetupVertexAttribs()
{
	// Specify the vertex attributes
	numAttributes = 4;
	attributeBuffers = new sce::Gnm::Buffer[numAttributes];

	InitAttributeBuffer(attributeBuffers[0], sce::Gnm::kDataFormatR32G32B32Float, &(vertexBuffer[0].position));
	InitAttributeBuffer(attributeBuffers[1], sce::Gnm::kDataFormatR32G32Float, &(vertexBuffer[0].textureCoord));
	InitAttributeBuffer(attributeBuffers[2], sce::Gnm::kDataFormatR32G32B32Float, &(vertexBuffer[0].normal));
	InitAttributeBuffer(attributeBuffers[3], sce::Gnm::kDataFormatR32G32B32Float, &(vertexBuffer[0].tangent));
}



void PS4Mesh::AllocateVertexMemory()
{
	vertices = new Vector3[numVertices];
	texCoords = new Vector2[numVertices];
	normals = new Vector3[numVertices];
	tangents = new Vector3[numVertices];
	indices = new int[numIndices];
}


void PS4Mesh::AssignQuadProperties()
{
	numVertices = 4;
	numIndices = 4;
	indexType = sce::Gnm::IndexSize::kIndexSize32;
	primitiveType = sce::Gnm::PrimitiveType::kPrimitiveTypeTriStrip;
}
void PS4Mesh::DefineQuadVertices()
{
	vertices[0] = Vector3(-1.0f, -1.0f, 0.0f);
	vertices[1] = Vector3(-1.0f, 1.0f, 0.0f);
	vertices[2] = Vector3(1.0f, -1.0f, 0.0f);
	vertices[3] = Vector3(1.0f, 1.0f, 0.0f);
}

void PS4Mesh::SetupQuadVectors()
{
	for (int i = 0; i < numVertices; ++i) {
		normals[i] = Vector3(0, 0, 1);
		tangents[i] = Vector3(1, 0, 0);
		indices[i] = i;
	}

}
void PS4Mesh::InitAttributeBuffer(sce::Gnm::Buffer &buffer, sce::Gnm::DataFormat format, void*offset)
{
	buffer.initAsVertexBuffer(offset, format, sizeof(MeshVertex), numVertices);
	buffer.setResourceMemoryType(sce::Gnm::kResourceMemoryTypeRO);
}

void PS4Mesh::Draw()
{
	std::cout << "PS4Mesh::Draw() on context " << currentGfxContext << std::endl;
	if (currentGfxContext)
	{
		texture->SetCurrentGFXContext(currentGfxContext);
		texture->Bind(0);
		//bumpTexture->Bind(1);
		currentGfxContext->setVertexBuffers(sce::Gnm::ShaderStage::kShaderStageVs, 0, numAttributes, attributeBuffers);
		currentGfxContext->setPrimitiveType(primitiveType);
		currentGfxContext->setIndexSize(indexType);
		currentGfxContext->drawIndex(numIndices, indexBuffer);
	}
}

void PS4Mesh::SetTexture(TextureBase * text)
{
	texture = static_cast<PS4Texture*>(text);
}

void PS4Mesh::DefineQuadTexCoords()
{
	texCoords[0] = Vector2(0.0f, 1.0f);
	texCoords[1] = Vector2(0.0f, 0.0f);
	texCoords[2] = Vector2(1.0f, 1.0f);
	texCoords[3] = Vector2(1.0f, 0.0f);
}

#endif
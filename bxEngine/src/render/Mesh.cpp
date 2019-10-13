#include "render/Mesh.h"
	
using namespace bbx;

Mesh::Mesh(std::vector<float> vertices, std::vector<float> texCoords, std::vector<float> normals, std::vector<unsigned int> indices) : vertices(vertices), textureCoords(texCoords), normals(normals), indices(indices)
{

	glGenVertexArrays(1, &VAO_ID);
	glGenBuffers(1, &VBO_ID);
	glGenBuffers(1, &IBO_ID);

	glBindVertexArray(VAO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);

	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), &vertexData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	/* positions ^ */

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	/* offsetof finds the offset of the glm::vec3 normal within struct Vertex */

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoord));

	glBindVertexArray(0);
// 	/* unbind */
}

// Mesh::Mesh(std::vector<Vertex> &data, std::vector<unsigned int> &indices, TextureList &textures) : vertexData(data), indices(indices), textureList(textures)
// {
// 	this->vertexData = data;
// 	this->indices = indices;

// 	this->numVertices = data.size();
// 	this->numIndices = indices.size();

// 	glGenVertexArrays(1, &VAO_ID);
// 	glGenBuffers(1, &VBO_ID);
// 	glGenBuffers(1, &IBO_ID);

// 	glBindVertexArray(VAO_ID);
// 	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);

// 	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), &vertexData[0], GL_STATIC_DRAW);

// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_ID);
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

// 	glEnableVertexAttribArray(0);
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
// 	/* positions ^ */

// 	glEnableVertexAttribArray(1);
// 	glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
// 	/* offsetof finds the offset of the glm::vec3 normal within struct Vertex */

// 	glEnableVertexAttribArray(2);
// 	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoord));

// 	glBindVertexArray(0);
// 	/* unbind */



// }

Mesh::~Mesh()
{

}

std::vector<Texture>& Mesh::getDiffuseTextures()
{
	return textureList.diffuse;
}
std::vector<Texture>& Mesh::getSpecTextures()
{
	return textureList.specular;
}

std::vector<float>& Mesh::getVertices()
{
	return vertices;
}

std::vector<float>& Mesh::getNormals()
{
	return normals;
}

std::vector<float>& Mesh::getTextureCoords()
{
	return textureCoords;
}

std::vector<unsigned int>& Mesh::getIndices()
{
	return indices;
}

unsigned int Mesh::getVAO_ID()
{
	return VAO_ID;
}

unsigned int Mesh::getVBO_ID()
{
	return VBO_ID;
}

unsigned int Mesh::getIBO_ID()
{
	return IBO_ID;
}

unsigned int Mesh::getNumVertices()
{
	return numVertices;
}

glm::mat4& Mesh::getTransform()
{
	return this->transform;
}

void Mesh::regenerateMesh()
{
	/* TODO: if the texture uv coordinates need to be remapped, regen mesh */
}
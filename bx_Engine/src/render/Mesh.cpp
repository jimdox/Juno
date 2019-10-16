#include "render/Mesh.h"
using namespace bbx;








Mesh::Mesh(std::vector<float> vertices, std::vector<float> texCoords, std::vector<float> normals, std::vector<unsigned int> indices) : vertices(vertices), textureCoords(texCoords), normals(normals), indices(indices)
{


}


Mesh::~Mesh()
{

}

void Mesh::addTexture(Texture* tex)
{
	Texture tx = *tex;
	this->textureList.diffuse.push_back(tx);
}

std::vector<Texture>& Mesh::getDiffuseTextures()
{
	return textureList.diffuse;
}
std::vector<Texture>& Mesh::getSpecTextures()
{
	return textureList.specular;
}

Texture& Mesh::getTextureZero()
{
	return textureList.diffuse[0];
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

unsigned int Mesh::getNumIndices()
{
	return this->numIndices;
}

unsigned int Mesh::getVAO_ID()
{
	return this->VAO_ID;
}

unsigned int Mesh::getNumVertices()
{
	return numVertices;
}



void Mesh::assignVAO(unsigned int id, unsigned int numIndices)
{
	this->VAO_ID = id;
	this->numIndices = numIndices;
}

void Mesh::regenerateMesh()
{
	/* TODO: if texture uv coordinates need to be remapped, reload mesh */
}
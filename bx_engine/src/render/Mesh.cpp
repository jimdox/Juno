#include "render/Mesh.h"
using namespace bx;





Mesh::Mesh()
{
	this->material.reflectivity = 0.05f;
	this->material.shineDamper = 1.0f;
}


Mesh::Mesh(std::vector<float> vertices, std::vector<float> texCoords, std::vector<float> normals, std::vector<unsigned int> indices) : vertices(vertices), textureCoords(texCoords), normals(normals), indices(indices)
{
	this->material.reflectivity = 0.6f;
	this->material.shineDamper = 0.85f;
}


Mesh::~Mesh()
{

}

void Mesh::addTexture(Texture* tex, TextureType tx_type)
{
	Texture tx = *tex;
	if(tx_type == TX_DIFFUSE)
	{
		this->textureList.diffuse.push_back(tx);
	}
	else if(tx_type == TX_SPECULAR)
	{
		this->textureList.specular.push_back(tx);
	}
}

void Mesh::setMaterial(Material& mat)
{
	this->material = mat;
}

Material& Mesh::getMaterial()
{
	return material;
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
	return this->normals;
}

std::vector<float>& Mesh::getTextureCoords()
{
	return this->textureCoords;
}

std::vector<unsigned int>& Mesh::getIndices()
{
	return this->indices;
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
	return this->numVertices;
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
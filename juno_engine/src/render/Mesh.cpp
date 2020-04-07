#include "render/Mesh.h"
using namespace juno;





// Mesh::Mesh()
// {
// 	this->material.reflectivity = 0.01f;
// 	this->material.shineDamper = 0.95f;
// }


Mesh::Mesh(unsigned int vao, unsigned int numIndices, unsigned int numVertices) : vao_id(vao), numIndices(numIndices), numVertices(numVertices)
{
	this->material.reflectivity = 0.2f;
	this->material.shineDamper = 0.95f;
	this->material.baseColor = glm::vec3(0.55f,0.55f, 0.55f);
	f_textured = false;
}


Mesh::~Mesh()
{

}

void Mesh::addTexture(Texture& tex)
{
	if(tex.getTexType() == TX_DIFFUSE)
	{
		if(!f_textured)
		{
			f_textured = true;
		}
		this->material.diffuseTextures.push_back(tex);
	}
	else if(tex.getTexType() == TX_SPECULAR)
	{
		this->material.specularTextures.push_back(tex);
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
	return material.diffuseTextures;
}
std::vector<Texture>& Mesh::getSpecTextures()
{
	return material.specularTextures;
}

unsigned int Mesh::getNumIndices()
{
	return this->numIndices;
}

unsigned int Mesh::getVaoID()
{
	return this->vao_id;
}

unsigned int Mesh::getNumVertices()
{
	return this->numVertices;
}

bool Mesh::isTextured()
{
	return f_textured;
}

void Mesh::assignVAO(unsigned int id, unsigned int numIndices, unsigned int numVertices)
{
	this->vao_id = id;
	this->numIndices = numIndices;
	this->numVertices = numVertices;
}

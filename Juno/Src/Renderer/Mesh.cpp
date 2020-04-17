#include "jnpch.h"
#include "Renderer/Mesh.h"
#include "Core/AssetManager.h"
using namespace Juno;


Mesh::Mesh(const std::string& filepath)
{	
	auto [id, indices, verts] = AssetManager::Get().LoadOBJFile(filepath);
	this->vaoID = id;
	this->numIndices = indices;
	this->numVertices = verts;
	f_textured = false;
}

Mesh::~Mesh()
{

}

// void Mesh::addTexture(Texture& tex)
// {
// 	if(tex.getTexType() == TX_DIFFUSE)
// 	{
// 		if(!f_textured)
// 		{
// 			f_textured = true;
// 		}
// 		this->material.diffuseTextures.push_back(tex);
// 	}
// 	else if(tex.getTexType() == TX_SPECULAR)
// 	{
// 		this->material.specularTextures.push_back(tex);
// 	}
// }

void Mesh::SetMaterial(Material& mat)
{
	this->material = mat;
}

Material& Mesh::GetMaterial()
{
	return material;
}

// std::vector<Texture>& Mesh::getDiffuseTextures()
// {
// 	return material.diffuseTextures;
// }
// std::vector<Texture>& Mesh::getSpecTextures()
// {
// 	return material.specularTextures;
// }

unsigned int Mesh::GetNumIndices()
{
	return this->numIndices;
}

unsigned int Mesh::GetVaoID()
{
	return this->vaoID;
}

unsigned int Mesh::GetNumVertices()
{
	return this->numVertices;
}

bool Mesh::IsTextured()
{
	return f_textured;
}

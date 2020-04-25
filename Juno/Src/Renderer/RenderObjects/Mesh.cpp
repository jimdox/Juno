#include "jnpch.h"
#include "Renderer/RenderObjects/Mesh.h"
#include "Core/AssetManager.h"
using namespace Juno;


Mesh::Mesh(const std::string& filepath)
{	
	auto [vertexArray, indices, verts] = AssetManager::Get().LoadOBJFile(filepath);
	this->vao = vertexArray;
	this->numIndices = indices;
	this->numVertices = verts;
	this->material = std::make_shared<Material>();
}

Mesh::~Mesh()
{
	delete vao;

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

void Mesh::SetMaterial(SPtr<Material> material)
{
	this->material = material;
}

SPtr<Material> Mesh::GetMaterial()
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

VertexArray* Mesh::GetVao()
{
	return vao;
}

unsigned int Mesh::GetNumVertices()
{
	return this->numVertices;
}

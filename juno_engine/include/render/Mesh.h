#pragma once
#include "pch.h"
#include "render/textures/Texture.h"
#include "render/Material.h"
#include "core/Asset.h"
namespace juno {


class Mesh : public Asset
{
public:
	//Mesh();
	Mesh(const std::string& filepath);

	// Mesh(const Mesh& mesh)
	// {
	// 	JN_WARN("Mesh Copy!");
	// 	this->vao_id = mesh.vao_id;
	// 	this->numIndices = mesh.numIndices;
	// 	this->numVertices = mesh.numVertices;
	// }

	virtual ~Mesh();
	
	//void addTexture(Texture& tex);
	void setMaterial(Material& mat);
	
	unsigned int getVaoID();
	// std::vector<Texture>& getDiffuseTextures();
	// std::vector<Texture>& getSpecTextures();
	std::string& getFilepath();
	Material& getMaterial();
	

	std::vector<float>& getVertices();
	std::vector<float>& getNormals();
	std::vector<float>& getTextureCoords();

	
	std::vector<unsigned int>& getIndices();
	unsigned int getNumIndices();
	unsigned int getNumVertices();
	bool isTextured();
	//float getFurthest();

	void assignVAO(unsigned int id, unsigned int numIndices, unsigned int numVertices);
	
protected:
	Material material;
	bool f_textured;
	unsigned int vaoID;
	unsigned int numVertices;
	unsigned int numIndices;

private: 
};

}
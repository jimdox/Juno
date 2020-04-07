#pragma once
#include "pch.h"
#include "render/textures/Texture.h"
#include "core/Log.h"


namespace juno {

struct Material 
{
	glm::vec3 baseColor = glm::vec3(0.5, 0.5, 0.5);
	float reflectivity;
	float shineDamper;
	std::vector<Texture> diffuseTextures;
	std::vector<Texture> specularTextures;
};


class Mesh
{
public:
	//Mesh();
	Mesh(unsigned int vao, unsigned int numIndices, unsigned int numVertices);

	// Mesh(const Mesh& mesh)
	// {
	// 	JN_WARN("Mesh Copy!");
	// 	this->vao_id = mesh.vao_id;
	// 	this->numIndices = mesh.numIndices;
	// 	this->numVertices = mesh.numVertices;
	// }

	virtual ~Mesh();
	
	void addTexture(Texture& tex);
	void setMaterial(Material& mat);
	
	unsigned int getVaoID();
	std::vector<Texture>& getDiffuseTextures();
	std::vector<Texture>& getSpecTextures();
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
	unsigned int vao_id;
	unsigned int numVertices;
	unsigned int numIndices;
};

}
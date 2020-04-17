#pragma once
//#include "Core/Asset.h"
#include "Renderer/Textures/Texture.h"
#include "Renderer/Material.h"
namespace Juno 
{
class Mesh 
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

	~Mesh();
	
	//void addTexture(Texture& tex);
	void SetMaterial(Material& mat);
	
	unsigned int GetVaoID();
	// std::vector<Texture>& getDiffuseTextures();
	// std::vector<Texture>& getSpecTextures();
	std::string& GetFilepath();
	Material& GetMaterial();
	

	std::vector<float>& GetVertices();
	std::vector<float>& GetNormals();
	std::vector<float>& GetTextureCoords();

	
	std::vector<unsigned int>& GetIndices();
	unsigned int GetNumIndices();
	unsigned int GetNumVertices();
	bool IsTextured();
	//float getFurthest();

	
protected:
	Material material;
	bool f_textured;
	unsigned int vaoID;
	unsigned int numVertices;
	unsigned int numIndices;

private: 

};
}
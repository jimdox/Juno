#pragma once
#include "pch.h"
#include "render/Texture.h"
#include "core/Log.h"


namespace bx {

typedef struct Material 
{
	float reflectivity;
	float shineDamper;

} Material;


/* Texture types are sequentially stored */
typedef struct TextureList
{
	std::vector<Texture> diffuse;
	std::vector<Texture> specular;
} TextureList;


class Mesh
{
public:
	Mesh();
	Mesh(std::vector<float> vertices, std::vector<float> textureCoords, std::vector<float> normals, std::vector<unsigned int> indices);

	virtual ~Mesh();
	
	void addTexture(Texture* tex, TextureType tx_type);
	void setMaterial(Material& mat);
	
	unsigned int getVAO_ID();
	std::vector<Texture>& getDiffuseTextures();
	std::vector<Texture>& getSpecTextures();
	Material& getMaterial();

	std::vector<float>& getVertices();
	std::vector<float>& getNormals();
	std::vector<float>& getTextureCoords();

	
	std::vector<unsigned int>& getIndices();
	unsigned int getNumIndices();
	unsigned int getNumVertices();
	//float getFurthest();

	void assignVAO(unsigned int id, unsigned int numIndices);
	
	void regenerateMesh();

private:

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> textureCoords;
	std::vector<unsigned int> indices;

	TextureList textureList;
	Material material;

	unsigned int VAO_ID;
	unsigned int numVertices;
	unsigned int numIndices;
	//float ptr_furthest;
};

}
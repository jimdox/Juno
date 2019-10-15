#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "render/Texture.h"
#include "core/Log.h"


namespace bbx {

typedef struct Material 
{
	// PBR related things will go here (reflectivity, shine damper, etc...)

} Material;

// typedef struct Vertex
// {
// 	glm::vec3 position; 
// 	glm::vec3 normal;
// 	glm::vec2 textureCoord;
// } Vertex;

typedef struct VAO_Data
{
	unsigned int VAO_ID;
	unsigned int numIndices;
} VAO_Data;

/* Texture types are sequentially stored */
typedef struct TextureList
{
	std::vector<Texture> diffuse;
	std::vector<Texture> specular;
} TextureList;


class Mesh
{
public:

	Mesh(std::vector<float> vertices, std::vector<float> textureCoords, std::vector<float> normals, std::vector<unsigned int> indices);

	virtual ~Mesh();
	
	void addTexture(Texture* tex);

	unsigned int getVAO_ID();


	std::vector<Texture>& getDiffuseTextures();
	std::vector<Texture>& getSpecTextures();
	Texture& getTextureZero();

	std::vector<float>& getVertices();
	std::vector<float>& getNormals();
	std::vector<float>& getTextureCoords();
	
	std::vector<unsigned int>& getIndices();
	unsigned int getNumIndices();
	unsigned int getNumVertices();

	void assignVAO(VAO_Data vao_data);
	
	void regenerateMesh();

private:

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> textureCoords;
	std::vector<unsigned int> indices;

	VAO_Data vaoData;
	TextureList textureList;

	unsigned int numVertices;
	unsigned int numIndices;
	
};

}
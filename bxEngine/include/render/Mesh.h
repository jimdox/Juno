#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "render/Texture.h"
#include "core/Log.h"
/*
	Stores geometry and texture data to be used when rendering mesh.
*/

namespace bbx {

typedef struct Material 
{
	// PBR related things will go here (reflectivity, shine damper, etc...)

} Material;

typedef struct Vertex
{
	glm::vec3 position; 
	glm::vec3 normal;
	glm::vec2 textureCoord;
} Vertex;

/* Texture types are sequentially stored */
typedef struct TextureList
{
	std::vector<Texture> diffuse;
	std::vector<Texture> specular;
} TextureList;


class Mesh
{
public:

	//Mesh(std::vector<float> vertices, std::vector<float> textureCoords, std::vector<float> normals, std::vector<unsigned int> indices);
	Mesh(std::vector<Vertex> data, std::vector<unsigned int> indices);
	virtual ~Mesh();
	
	void addTexture(Texture* tex);

	unsigned int getVAO_ID();
	unsigned int getVBO_ID(unsigned int n);
	unsigned int getIBO_ID();

	std::vector<Texture>& getDiffuseTextures();
	std::vector<Texture>& getSpecTextures();

	// std::vector<float>& getVertices();
	// std::vector<float>& getNormals();
	// std::vector<float>& getTextureCoords();
	
	std::vector<unsigned int>& getIndices();
	unsigned int getNumVertices();
	
	//void setTransform(glm::mat4& transf);
	glm::mat4& getTransform();
	void regenerateMesh();
	//static Mesh loadMesh();

private:

	// std::vector<float> &vertices;
	// std::vector<float> &normals;
	// std::vector<float> &textureCoords;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
	TextureList textureList;

	glm::mat4 transform;
	unsigned int numVertices;
	unsigned int numIndices;
	unsigned int VAO_ID, VBO_ID, IBO_ID;
	
	// unsigned int VBO_IDs[3];
	/* Vertex Array, Vertex Buffer, Index Buffer Objects */
};

}
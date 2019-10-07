#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "render/Texture.h"
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

/* Texture refs are separately stored */
class TextureList
{
public:
	TextureList(std::vector<Texture> &diff, std::vector<Texture> &spec) : diffuse(diff), specular(spec){}
	std::vector<Texture> &diffuse;
	std::vector<Texture> &specular;
};


class Mesh
{
public:
	Mesh(std::vector<Vertex> &data, std::vector<unsigned int> &indices, TextureList &textures);
	virtual ~Mesh();
	
	unsigned int getVAO_ID();
	unsigned int getVBO_ID();
	unsigned int getIBO_ID();

	std::vector<Texture>& getDiffuseTextures();
	std::vector<Texture>& getSpecTextures();

	std::vector<Vertex>& getVertices();
	std::vector<unsigned int>& getIndices();
	unsigned int getNumVertices();
	
	glm::mat4& getTransform();
	void regenerateMesh();

private:

	std::vector<Vertex> &vertexData;
	std::vector<unsigned int> &indices;
	
	std::vector<bbx::Texture> &diffuseTextures;
	std::vector<bbx::Texture> &specularTextures;

	glm::mat4 transform;
	unsigned int numVertices;
	unsigned int numIndices;
	unsigned int VAO_ID, VBO_ID, IBO_ID;
	/* Vertex Array, Vertex Buffer, Index Buffer Objects */
};

}
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
	glm::vec3 &position; 
	glm::vec3 &normal;
	glm::vec2 &textureCoord;
} Vertex;


class Mesh
{
public:
	Mesh(std::vector<Vertex> data, std::vector<unsigned int> &indices, std::vector<Texture> &textures);
	virtual ~Mesh();
	


	glm::mat4& getTransform();
	unsigned int getNumVertices();

	void regenerateMesh();

private:

	std::vector<Vertex> &meshData;
	std::vector<unsigned int> &indices;
	std::vector<Texture> &textures;

	glm::mat4 transform;
	unsigned int numVertices;
	unsigned int numIndices;
	unsigned int VAO_ID, VBO_ID, IBO_ID;
	/* Vertex Array, Vertex Buffer, Index Buffer Objects */
};

}
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/*
	Stores geometry and texture data to be used when rendering mesh.
*/

namespace bbx {

struct Material {
	// PBR related things go here (reflectivity, shine damper, etc...)

};



class Mesh
{
public:
	Mesh(std::vector<float> &vrtCoords, std::vector<float> &texCoords, std::vector<unsigned int> &indices);
	virtual ~Mesh();
	std::vector<float>& getVertexCoords();
	std::vector<float>& getTextureCoords();
	std::vector<unsigned int>& getIndices();
	glm::mat4& getTransform();
	unsigned int& getNumVertices();


private:
	unsigned int numVertices;
	std::vector<float> vertexCoords;
	std::vector<float> textureCoords;
	std::vector<unsigned int> indices;
	glm::mat4 transform;
};

}
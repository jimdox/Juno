#include "render/Mesh.h"
	
using namespace bbx;

Mesh::Mesh(std::vector<float> &vertCoords, std::vector<float> &textureCoords, std::vector<unsigned int> &indices)
{
	this->vertexCoords = vertCoords;
	this->textureCoords = textureCoords;
	this->indices = indices;
	this->numVertices = vertexCoords.size();
}

Mesh::~Mesh()
{

}

std::vector<float>& Mesh::getVertexCoords()
{
	return vertexCoords;
}

std::vector<float>& Mesh::getTextureCoords()
{
	return textureCoords;
}
std::vector<unsigned int>& Mesh::getIndices()
{
	return indices;
}

unsigned int& Mesh::getNumVertices()
{
	return numVertices;
}
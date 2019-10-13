#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "render/Mesh.h"

namespace bbx {
class Entity
{
public:
	Entity(std::string& filepath, std::vector<Mesh>& meshes);
	Entity(std::string& filepath);
	~Entity();

	void loadModelData(std::string& filepath);

	

	std::vector<Mesh>& getMeshList();


private:
	std::vector<Mesh> meshList;
	std::string& assetFilePath;

};
}


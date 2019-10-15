#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "render/Mesh.h"

namespace bbx {
class Entity
{
public:
	Entity(Mesh& mesh, std::string& name);
	Entity(Mesh& mesh, glm::vec3& pos, glm::vec3& rot, float scale, std::string& name);	
	//Entity(std::string& filepath);
	~Entity();

	Mesh& getMesh();
	glm::vec3& getPosition();
	glm::vec3& getRotation();
	float getScale();

	void setPosition(glm::vec3& pos);
	void setRotation(glm::vec3& rot);
	void setScale(float scale);

	void addPosition(glm::vec3& dpos);
	void addRotation(glm::vec3& drot);

private:
	std::string& name;
	Mesh mesh;
	glm::vec3 position;
	glm::vec3 rotation;
	float scale;
	//std::string& assetFilePath;

};
}


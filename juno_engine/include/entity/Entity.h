#pragma once
#include "pch.h"
#include "render/Mesh.h"

namespace juno {
class Entity
{
public:
	Entity(Mesh& mesh, const std::string& name);
	Entity(Mesh& mesh, const glm::vec3& pos, const glm::vec3& rot, float scale, const std::string& name);	
	~Entity();

	Mesh& getMesh();
	glm::vec3& getPosition();
	glm::vec3& getRotation();
	float getScale();
	std::string& getName();

	void setPosition(const glm::vec3& pos);
	void setRotation(const glm::vec3& rot);
	void setScale(float scale);

	void addPosition(const glm::vec3& delta_pos);
	void update();

	float delta_x = 0.0f;

private:
	std::string name;
	Mesh mesh;
	glm::vec3 position;
	glm::vec3 rotation;
	
	float scale;
	bool visible;
};
}


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
	glm::vec3& getVelocity();
	glm::vec3& getRotVelocity();
	float getScale();
	std::string& getName();

	void setPosition(const glm::vec3& pos);
	void setRotation(const glm::vec3& rot);
	void setVelocity(const glm::vec3& vel);
	void setRotVelocity(const glm::vec3& rvel);
	void setScale(float scale);

	void addPosition(const glm::vec3& delta_pos);
	void update();

	float delta_x = 0.0f;

private:
	std::string name;
	Mesh mesh;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 velocity;
	glm::vec3 rot_velocity;
	
	float scale;
	bool visible;
};
}


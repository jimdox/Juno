#pragma once
#include "Renderer/Mesh.h"

namespace Juno {
class Entity
{
public:
	Entity(Mesh& mesh, const std::string& name);
	Entity(Mesh& mesh, const glm::vec3& pos, const glm::vec3& rot, float scale, const std::string& name);	
	~Entity();

	Mesh& GetMesh();
	glm::vec3& GetPosition();
	glm::vec3& GetRotation();
	glm::vec3& GetVelocity();
	glm::vec3& GetRotVelocity();
	float GetScale();
	std::string& GetName();

	void SetPosition(const glm::vec3& pos);
	void SetRotation(const glm::vec3& rot);
	void SetVelocity(const glm::vec3& vel);
	void SetRotVelocity(const glm::vec3& rvel);
	void SetScale(float scale);

	void AddPosition(const glm::vec3& delta_pos);
	void Update();

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


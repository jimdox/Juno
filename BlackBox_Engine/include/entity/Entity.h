#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "render/Mesh.h"

namespace bbx {
class Entity
{
public:
	Entity(Mesh& mesh, glm::mat4& transform);
	~Entity();

	Mesh& getMesh();
	glm::mat4& getPosition();


private:
	glm::mat4& transformMatrix;
	Mesh& mesh;
};
}


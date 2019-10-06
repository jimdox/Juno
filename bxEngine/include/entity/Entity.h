#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "render/Mesh.h"

namespace bbx {
class Entity
{
public:
	Entity(Mesh& mesh);
	~Entity();

	Mesh& getMesh();
	//glm::mat4& getPosition();


private:
	Mesh& mesh;
};
}


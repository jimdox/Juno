#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "render/Mesh.h"

namespace bbx {
class Entity
{
public:

private:
	glm::mat4& transformMatrix;
	Mesh mesh;
};
}


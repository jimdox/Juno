#pragma once
#include <glm/glm.hpp>
/* wrapper for vector math in case I ever decide to write my own math lib with SIMD */

namespace juno 
{
#define Vec2 glm::vec2
#define Vec3 glm::vec3
#define Vec4 glm::vec4
}
#include "jnpch.h"
#include "Renderer/Light.h"
//#include <glm/glm.hpp>

using namespace Juno;

Light::Light(const glm::vec3& pos, const glm::vec3& color)
{
    this->position = pos;
    this->color = color;
    this->attenuationFactors = glm::vec3(1.0f,0.0f,0.0f);
}

Light::Light(const glm::vec3& pos, const glm::vec3& color, const glm::vec3& a)
{
    this->position = pos;
    this->color = color;
    this->attenuationFactors = a;
}

Light::~Light()
{

}

glm::vec3& Light::GetPosition()
{
    return position;
}

glm::vec3& Light::GetColor()
{
    return color;
}

glm::vec3& Light::GetAttenuation()
{
    return this->attenuationFactors;
}

void Light::SetPosition(const glm::vec3& pos)
{
    this->position = pos;
}

void Light::SetColor(glm::vec3 color)
{
    this->color = color;
}

void Light::SetAttenuation(const glm::vec3& a)
{
    this->attenuationFactors = a;
}
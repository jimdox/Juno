#include "render/lights/Light.h"

using namespace juno;

Light::Light(const glm::vec3& pos, const glm::vec3& color)
{
    this->position = pos;
    this->color = color;
    this->attenuation_factors = glm::vec3(1.0f,0.0f,0.0f);
}

Light::Light(const glm::vec3& pos, const glm::vec3& color, const glm::vec3& a)
{
    this->position = pos;
    this->color = color;
    this->attenuation_factors = a;
}

Light::~Light()
{

}

glm::vec3& Light::getPosition()
{
    return position;
}

glm::vec3& Light::getColor()
{
    return color;
}

glm::vec3& Light::getAttenuation()
{
    return this->attenuation_factors;
}

void Light::setPosition(glm::vec3 pos)
{
    this->position = pos;
}

void Light::setColor(glm::vec3 color)
{
    this->color = color;
}

void Light::setAttenuation(const glm::vec3& a)
{
    this->attenuation_factors = a;
}


#include "render/lights/Light.h"

using namespace bbx;

Light::Light(glm::vec3 pos, glm::vec3 color)
{
    this->position = pos;
    this->color = color;
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

void Light::setPosition(glm::vec3 pos)
{
    this->position = pos;
}

void Light::setColor(glm::vec3 color)
{
    this->color = color;
}


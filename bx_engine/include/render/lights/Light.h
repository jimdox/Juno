#pragma once
#include "pch.h"
namespace bx {

class Light
{

public:
    Light(glm::vec3 pos, glm::vec3 color);
    ~Light();

    glm::vec3& getPosition();
    glm::vec3& getColor();
    void setPosition(glm::vec3 pos);
    void setColor(glm::vec3 color);

private:
    glm::vec3 position;
    glm::vec3 color;

};
}
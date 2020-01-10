#pragma once
#include "pch.h"
namespace juno {

class Light
{

public:
    Light(const glm::vec3& pos, const glm::vec3& color);
    Light(const glm::vec3& pos, const glm::vec3& color, const glm::vec3& a);
    ~Light();

    glm::vec3& getPosition();
    glm::vec3& getColor();
    glm::vec3& getAttenuation();
    void setPosition(glm::vec3 pos);
    void setColor(glm::vec3 color);
    void setAttenuation(const glm::vec3& a);

private:
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 attenuation_factors;
};
}
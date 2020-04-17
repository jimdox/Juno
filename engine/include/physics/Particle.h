#pragma once
#include <glm/glm.hpp>
namespace juno
{
class Particle
{
public:
    Particle();
    Particle(const glm::vec3& pos, const glm::vec3& vel, float mass);

    glm::vec3& getPosition();
    glm::vec3& getVelocity();
    float getMass();

    void setPosition(const glm::vec3& position);
    void setVelocity(const glm::vec3& velocity);
    void setMass(float mass);




protected:
    glm::vec3 position;
    glm::vec3 velocity;
    float mass;

};
}
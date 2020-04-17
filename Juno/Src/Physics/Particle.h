#pragma once
#include <glm/glm.hpp>
namespace Juno
{
class Particle
{
public:
    Particle();
    Particle(const glm::vec3& pos, const glm::vec3& vel, float mass);

    glm::vec3& GetPosition();
    glm::vec3& GetVelocity();
    float GetMass();

    void SetPosition(const glm::vec3& position);
    void SetVelocity(const glm::vec3& velocity);
    void SetMass(float mass);


protected:
    glm::vec3 position;
    glm::vec3 velocity;
    float mass;

};
}
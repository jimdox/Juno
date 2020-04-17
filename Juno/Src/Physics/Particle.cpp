#include "jnpch.h"
#include "Physics/Particle.h"   
using namespace Juno;


Particle::Particle()
{
    mass = 1.0f;
}

Particle::Particle(const glm::vec3& pos, const glm::vec3& vel, float mass) : position(pos), velocity(vel), mass(mass){}



glm::vec3& Particle::GetPosition()
{
    return position;
}

glm::vec3& Particle::GetVelocity()
{
    return velocity;
}

float Particle::GetMass()
{
    return mass;
}

void Particle::SetPosition(const glm::vec3& position)
{
    this->position = position;
}

void Particle::SetVelocity(const glm::vec3& velocity)
{
    this->velocity = velocity;
}

void Particle::SetMass(float mass)
{
    this->mass = mass;
}

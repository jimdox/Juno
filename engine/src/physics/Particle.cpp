#include "pch.h"
#include "physics/Particle.h"   
using namespace juno;


Particle::Particle()
{
    mass = 1.0f;
}

Particle::Particle(const glm::vec3& pos, const glm::vec3& vel, float mass) : position(pos), velocity(vel), mass(mass){}



glm::vec3& Particle::getPosition()
{
    return position;
}

glm::vec3& Particle::getVelocity()
{
    return velocity;
}

float Particle::getMass()
{
    return mass;
}

void Particle::setPosition(const glm::vec3& position)
{
    this->position = position;
}

void Particle::setVelocity(const glm::vec3& velocity)
{
    this->velocity = velocity;
}

void Particle::setMass(float mass)
{
    this->mass = mass;
}

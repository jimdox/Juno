#pragma once
#include "Utils/Math.h"
namespace Juno
{
class Particle
{
public:
    Particle();
    Particle(const Vec3& pos, const Vec3& vel, float mass);

    Vec3& GetPosition();
    Vec3& GetVelocity();
    float GetMass();

    void SetPosition(const Vec3& position);
    void SetVelocity(const Vec3& velocity);
    void SetMass(float mass);


protected:
    Vec3 position;
    Vec3 velocity;
    float mass;

};
}
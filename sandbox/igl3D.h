#pragma once
#include "entity/Entity.h"
#include <glm/glm.hpp>


static const unsigned int NUM_PARTICLES = 1000;
static const float BOX_LENGTH = 10;
static const float DT = 1e-4;
static const float G = 9.81;
static const float Y = 12;
static const float U = 6;
static const float R_O = 0.25;
static const float KD = 1e-1;
static const unsigned int TIME_SKIP = 100;

static const float SPHERE_RADIUS = 0.1;
static const double BOX_VOLUME = BOX_LENGTH * BOX_LENGTH * BOX_LENGTH;

double dabs(double x)
{
    if(x < 0)
        return -x;
    return x;
}

class Particle : public juno::Entity
{
public:
    Particle(juno::Mesh& mesh, const glm::vec3& position, const glm::vec3& rotation, 
                                                float mass, const std::string& name) : Entity(mesh, position, rotation, SPHERE_RADIUS, name){}
    
    
    float getMass()
    {
        return mass;
    }
    
    glm::vec3& getVelocity()
    {
        return velocity;
    }

    void setVelocity(const glm::vec3& velocity)
    {
        this->velocity = velocity;
    }

    
private:
    glm::vec3 velocity;
    float mass;
};

void computeForce(std::vector<Particle>& bois, double& potential)
{
    int i, j;
    glm::vec3 force, p;
    double mag;

    for(i = 0; i < NUM_PARTICLES; i++)
    {
        for(j = i + 1; j < NUM_PARTICLES; j++)
        {
            p = bois[i].getPosition() - bois[j].getPosition();
            mag = p.length();
            if(mag < 5 * R_O)
            {
                force = p * 24.0f * ((float)(pow(R_O, Y) * pow(mag, -Y) - pow(R_O, U) * pow(mag, -U)));
                bois[i].setVelocity(bois[i].getVelocity() + force * (DT / bois[i].getMass()));
                bois[j].setVelocity(bois[j].getVelocity() - force * (DT / bois[j].getMass()));
            }
        }
        bois[i].setVelocity(bois[i].getVelocity() - bois[i].getVelocity() * (KD * DT / bois[i].getMass()) );
    }
}

void updatePosition(std::vector<Particle>& bois, double& momentum, double& potential)
{
    for(int i = 0; i < NUM_PARTICLES; i++)
    {
        bois[i].addPosition(bois[i].getVelocity() * (DT/2));
    }

    computeForce(bois, potential);

    for(int i = 0; i < NUM_PARTICLES; i++)
    {
        bois[i].addPosition(bois[i].getVelocity() * (DT/2));
    }
}

void updateSimulation(std::vector<Particle>& bois, int& step, double& time, double* info)
{

}

void updateInfo(double& momentum, double& potential, double& kinetic, double info[])
{
    double PV = BOX_VOLUME * momentum / (DT * TIME_SKIP * BOX_LENGTH * BOX_LENGTH * 6);
    double NkT = NUM_PARTICLES * kinetic * 2.0 / 3.0;
    double ratio = PV / NkT;
    info[0] = PV;
    info[1] = NkT;
    info[2] = ratio;
    momentum = 0;
    potential = 0;
    kinetic = 0;
}

void run_sim()
{
    double time = 0;
    int i, j, k, l;
    int step = 0;
    double P = 0;
    double potential, momentum, kinetic;
    double data[3];
    double Vo = 100;

    std::vector<Particle> particles;
    particles.reserve(NUM_PARTICLES);

    //for(int i = )
}
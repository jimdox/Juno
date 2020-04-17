#pragma once
#include "pch.h"
#include "render/shaders/Shader.h"
#include "physics/Particle.h"

namespace juno
{

class ComputeShader : public Shader
{
public:
    ComputeShader(std::vector<Particle>& particles, const std::string& fp, unsigned int numWG);
    void setup(std::vector<Particle>& particles);
    void bindCS();
    void run();


    GLuint getCSID();
    GLuint getParticleSSBO();
    GLuint getParticleVaoID();
    unsigned int getNumObjects();
    unsigned int getNumWorkGroups();

private:
    void compileCShader();

    GLuint csProgID;
    GLuint computeVaoID;
    GLuint computeSSBO;
    GLuint loc_particleBuffer;
    unsigned int numWorkGroups;    
    unsigned int numObjects;
    
};
}
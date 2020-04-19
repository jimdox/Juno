#pragma once
#include "Renderer/Shaders/Shader.h"
#include "Physics/Particle.h"
namespace Juno
{

class ComputeShader : public Shader
{
public:
    ComputeShader(std::vector<Particle>& particles, const std::string& fp, unsigned int numWG);
    void Setup(std::vector<Particle>& particles);
    
    void BindCS();
    void Compute();

    GLuint GetCSID();
    GLuint GetParticleSSBO();
    GLuint GetParticleVaoID();
    unsigned int GetNumObjects();
    unsigned int GetNumWorkGroups();

private:
    void CompileCShader();

    GLuint csProgID;
    GLuint computeVaoID;
    GLuint computeSSBO;
    GLuint loc_particleBuffer;
    unsigned int numWorkGroups;    
    unsigned int numObjects;
    
};
}
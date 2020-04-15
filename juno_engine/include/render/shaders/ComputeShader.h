#pragma once
#include "pch.h"
#include "render/shaders/Shader.h"

struct Particle
{
    glm::vec3 position; 
    glm::vec3 velocity; 
    float mass;
    float scale;
};


namespace juno
{

class ComputeShader : public Shader
{
public:
    ComputeShader(const std::string& fp, unsigned int numObjects, unsigned int wgSize);
    void setup();
    void bind();
    unsigned int getNumObjects();
    unsigned int getCSID();

    GLuint getParticleSSBO();
    std::vector<Particle>& getParticles();

    GLuint getParticleVaoID();


private:
    void compileCShader();
    GLuint csProgID;
    GLuint computeVaoID;
    GLuint computeSSBO;

    GLuint loc_particleBuffer;


    unsigned int numObjects;
    unsigned int workGroupSize;
    
    std::vector<Particle> particles;


};
}
#pragma once
#include "pch.h"
#include "render/shaders/Shader.h"

namespace juno
{

class ComputeShader : public Shader
{
public:
    ComputeShader(const std::string& fp, unsigned int numObjects, unsigned int wgSize);
    void init();
    void bind();
    unsigned int getNumObjects();
    unsigned int getCSID();

    GLuint getPositionBufferID();
    GLuint getVelocityBufferID();
    GLuint getColorBufferID();
    //GLuint getForceBufferID();

    std::vector<glm::vec4>& getPositions();
    std::vector<glm::vec4>& getVelocities();
    std::vector<glm::vec4>& getColors();
    std::vector<glm::vec4>& getForces();

private:
    void compileCShader();
    GLuint csProgID;
    GLuint dataBuffers[4];      /* shader storage buffer objects */
    GLuint positionBufferTexID;
    GLuint dispatchBufferID;

    GLuint loc_positionBuffer;
    GLuint loc_velocityBuffer;
    GLuint loc_colorBuffer;
    GLuint loc_forceBuffer;
    GLuint loc_lifeBuffer;

    unsigned int numObjects;
    unsigned int workGroupSize;
    
    std::vector<glm::vec4> positions;
    std::vector<glm::vec4> velocities;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec4> forces;
    std::vector<float> lifetimes;

};
}
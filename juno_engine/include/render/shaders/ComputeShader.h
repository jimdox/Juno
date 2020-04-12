#pragma once
#include "pch.h"
#include "render/shaders/Shader.h"

namespace juno
{

struct posVector
{
    float x, y, z, w;
};

struct velVector
{
    float vx, vy, vz, vw;
};

struct colorComponent
{
    float r, g, b, a;
};

class ComputeShader : public Shader
{
public:
    ComputeShader(const std::string& fp, unsigned int numObjects, unsigned int wgSize);
    ~ComputeShader();

    void compileShader();

private:

    unsigned int numObjects;
    unsigned int workGroupSize;
    GLuint ssBufferObjects[3];

};
}
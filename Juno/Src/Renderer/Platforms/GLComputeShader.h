#pragma once
#include "Renderer/Data/Shader.h"
#include "Renderer/Platforms/GLShader.h"
#include "Physics/Particle.h"
#include <string>
#include <array>
#include <vector>
namespace Juno
{
class GLComputeShader : public GLShader
{
public:
    GLComputeShader(std::vector<Particle>& particles, const std::string& fp, unsigned int numWG);
    void Setup(std::vector<Particle>& particles);
    void Prepare();

    void BindCS();
    void Compute();

    unsigned int GetCSID();
    unsigned int GetParticleSSBO();
    unsigned int GetParticleVaoID();
    unsigned int GetNumObjects();
    unsigned int GetNumWorkGroups();

    void SetViewMatrix(const Mat4& viewMat);
    void SetProjMatrix(const Mat4& projMat);

private:
    std::array<ShaderComponentType, 3> renderShaderComponents = { ShaderComponentType::Vertex, ShaderComponentType::Fragment, ShaderComponentType::Empty };
    unsigned int csProgID;
    unsigned int computeVaoID;
    unsigned int computeSSBO;
    unsigned int numWorkGroups;    
    unsigned int numObjects;

    unsigned int loc_particleBuffer;
    unsigned int loc_viewMatrix;
    unsigned int loc_projMatrix;
    
};
}
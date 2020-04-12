#include "render/shaders/ComputeShader.h"
#include "core/AssetManager.h"
using namespace juno;

ComputeShader::ComputeShader(const std::string& fp, unsigned int numObjects, unsigned int wgSize)
{
    this->filepath = fp;
    this->numObjects = numObjects;
    this->workGroupSize = wgSize;
}

void ComputeShader::compileShader()
{
    GLuint vertShader = AssetManager::get().loadShaderFile(filepath, GL_VERTEX_SHADER);
    GLuint fragShader = AssetManager::get().loadShaderFile(filepath, GL_FRAGMENT_SHADER);
    GLuint compShader = AssetManager::get().loadShaderFile(filepath, GL_COMPUTE_SHADER);

    this->progID = glCreateProgram();

    glAttachShader(progID, vertShader);
    glAttachShader(progID, fragShader);
    glAttachShader(progID, compShader);
    
    JN_INFO("Linking Compute Shader");
    
    glLinkProgram(progID);

    


}
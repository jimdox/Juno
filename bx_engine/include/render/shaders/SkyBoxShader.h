#pragma once
#include "pch.h"
#include "render/shaders/Shader.h"

namespace bx{
class SkyBoxShader : public Shader
{
public:
    SkyBoxShader();
    SkyBoxShader(const std::string& filepath);
    virtual ~SkyBoxShader();

    bool loadShader(const std::string& filepath);
    void bindAllAttribs();
    void cacheUniformLocations();


protected:
    const std::string DEFAULT_PATH = "./bx_engine/res/shaders/skybox";

};
}
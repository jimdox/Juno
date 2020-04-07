#include "render/shaders/SkyBoxShader.h"

using namespace juno;

SkyBoxShader::SkyBoxShader(const std::string& filepath)
{
    loadShader(filepath);
    loc_projectionMatrix = glGetUniformLocation(progID, "projection");
    loc_viewMatrix = glGetUniformLocation(progID, "view");
}

SkyBoxShader::~SkyBoxShader()
{

}



#pragma once 
#include <vector>
#include "Renderer/RenderObjects/Textures/CubeMap.h"
namespace Juno
{
class SkyBox 
{
public:
    SkyBox();
    SkyBox(CubeMap& cubeMap);
    ~SkyBox();
    unsigned int GetVaoID();
    unsigned int GetNumVertices();

private:
    unsigned int vao_id;
    std::vector<float> vertices;
};
}
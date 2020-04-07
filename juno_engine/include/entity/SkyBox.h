#pragma once 
#include <vector>
#include "render/textures/CubeMap.h"
namespace juno
{
class SkyBox 
{
public:
    SkyBox();
    SkyBox(CubeMap& cubeMap);
    ~SkyBox();
    unsigned int getVaoID();
    unsigned int getNumVertices();
private:
    unsigned int vao_id;
    std::vector<float> vertices;

};
}
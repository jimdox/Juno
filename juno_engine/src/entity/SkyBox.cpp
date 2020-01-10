#include "entity/SkyBox.h"

using namespace juno;

SkyBox::SkyBox(CubeMap& cubeMap) : cubeMap(cubeMap)
{

}

SkyBox::~SkyBox()
{

}

CubeMap& SkyBox::getCubeMap()
{
    return cubeMap;
}

std::vector<float>& SkyBox::getVertices()
{
    return VERTICES;
}

unsigned int SkyBox::getNumVertices()
{
    return VERTICES.size();
}

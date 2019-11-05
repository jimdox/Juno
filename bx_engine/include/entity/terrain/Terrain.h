#pragma once
#include "render/Mesh.h"
#include "render/textures/Texture.h"
namespace bx {
class Terrain
{
public:
    Terrain(float x_coord, float z_coord, Texture& texture);
    ~Terrain();

    float getXCoord();
    float getZCoord();
    Mesh& getMesh();

private:
    const float BLOCK_SIZE = 800;
    const float NUM_VERTICES = 128;
    float x, z;
    Mesh mesh;
    Texture texture;
};
}
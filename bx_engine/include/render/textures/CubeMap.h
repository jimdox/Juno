#pragma once
#include "render/textures/Texture.h"
#include <string>
#include <array>

namespace bx {
class CubeMap : Texture
{
public:
    CubeMap(std::array<const std::string, 6> texturePaths, TextureType txType);
    ~CubeMap();

    std::array<const std::string, 6>& getFilepaths();
    

private:
    std::array<const std::string, 6> filepaths;

};

}
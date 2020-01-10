#pragma once 
#include <glm/glm.hpp>
#include <string>
#include "render/textures/Texture.h"
namespace juno{
class GuiImage : public Texture
{
public:
    GuiImage(const std::string&, glm::vec2 pos, glm::vec2 sc);
    ~GuiImage();
    glm::vec2& getPosition();
    glm::vec2& getScale();
    void setPosition(glm::vec2& pos);
    void setScale(glm::vec2& scale);

private:
    unsigned int textureID;
    glm::vec2 position;
    glm::vec2 scale;
};
}
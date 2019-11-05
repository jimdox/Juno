#include "gui/GuiImage.h"
#include "core/AssetLoader.h"
using namespace bx;

GuiImage::GuiImage(const std::string& filepath, glm::vec2 pos, glm::vec2 sc) : Texture(filepath, TX_DIFFUSE)
{
    //textureID = bxImport::loadTexture(filepath, GL_TEXTURE_2D, TX_DIFFUSE);


}
GuiImage::~GuiImage()
{
    
}


glm::vec2& GuiImage::getPosition()
{
    return position;
}
glm::vec2& GuiImage::getScale()
{
    return scale;
}
void GuiImage::setPosition(glm::vec2& pos)
{
    this->position = pos;
}
void GuiImage::setScale(glm::vec2& scale)
{
    this->scale = scale;
}
#pragma once 
namespace Juno
{
class VKTexture : public Texture
{
public:
    VKTexture(TextureType texType, bool transparency);
    ~VKTexture();
    void Bind() override;
    void Unbind() override;
    void Load(unsigned char** imageData, int width, int height) override;

private:

};
}
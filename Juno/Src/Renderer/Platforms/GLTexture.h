#pragma once
namespace Juno 
{
class GLTexture : public Texture
{
public:
    GLTexture(TextureType texType, bool transparency);
    ~GLTexture();

    void Bind() override;
    void Unbind() override;
    void Load(unsigned char** imageData, int width, int height) override;

private:
    unsigned int textureID;

};
}
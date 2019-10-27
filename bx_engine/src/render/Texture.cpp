#include "render/Texture.h"
#include "core/Log.h"
#include "stb_image.h"
#include "core/AssetLoader.h"
using namespace bx;

/* this constructor loads the texture itself */
Texture::Texture(const std::string& filepath, TextureType tx_type) : filepath(filepath), tex_type(tex_type)
{
	transparency = false;
	textureID = bxImport::loadTexture(filepath, tex_type);
}

/* called by bxAssetLoader */
Texture::Texture(const std::string& filepath, TextureType tx_type, bool transp) : filepath(filepath), tex_type(tx_type), transparency(transp)
{	
	textureID = bxImport::loadTexture(filepath, tex_type);
}

Texture::~Texture()
{

}

GLuint Texture::getID()
{
	return this->textureID;
}

bool Texture::containsTransparency()
{
	return transparency;
}

void Texture::setTransparency(bool flag)
{
	this->transparency = flag;
}

const std::string& Texture::getFilePath()
{
	return filepath;
}

TextureType Texture::getTexType()
{
	return tex_type;
}

/*
Texture::Texture(const Texture& other)  : filepath(other.getFilePath()), tex_type(other.gettex_type())// copy const.
{

}
*/

Texture& Texture::operator=(const Texture& rhs)  // assignment op
{
	if (this == &rhs) return *this;
	//assignment operator
	return *this;
}

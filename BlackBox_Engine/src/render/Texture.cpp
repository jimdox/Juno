#include "render/Texture.h"
#include "core/Log.h"
#include "stb_image.h"
#include <iostream>
using namespace bbx;

Texture::Texture(const std::string& filepath)
{
	int width, height, channels;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);


	unsigned char* imageData = stbi_load((filepath.c_str()), &width, &height, &channels, 0);

	if (imageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		BBox::Log::clientLog()->error("Failed to load image: " + filepath);
	}
	stbi_image_free(imageData);

}

Texture::~Texture()
{

}

GLuint Texture::getID()
{
	return this->textureID;
}

float Texture::getReflectivity()
{
	return this->reflectivity;
}

float Texture::getShineDamper()
{
	return this->shineDamper;
}

void Texture::setReflectivity(float& r)
{
	this->reflectivity = r;
}

void Texture::setShineDamper(float& sd)
{
	this->shineDamper = sd;
}



Texture::Texture(const Texture& other)  // copy const.
{

}

Texture& Texture::operator=(const Texture& rhs)  // assignment op
{
	if (this == &rhs) return *this;
	//assignment operator
	return *this;
}

#pragma once
#include <GL/glew.h>
#include <string>
#include "stb_image.h"

namespace bx {
class Texture
{
public:
	Texture(std::string& filepath, std::string& t, bool hasUVs);
	Texture(std::string& dir, std::string& type);
	virtual ~Texture();

	std::string& getFilePath();
	std::string& getTexType();

	unsigned int getID();
	float getReflectivity();
	float getShineDamper();
	void setReflectivity(float& r);
	void setShineDamper(float& sd);

	//Texture(const Texture& other);
	Texture& operator=(const Texture& other);
	

private:
	unsigned int textureID;
	float shineDamper;
	float reflectivity;
	std::string& filepath;
	std::string& texType;




	};
}



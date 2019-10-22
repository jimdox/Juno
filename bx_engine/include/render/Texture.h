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

	bool containsTransparency();
	void setTransparency(bool flag);

	unsigned int getID();


	//Texture(const Texture& other);
	Texture& operator=(const Texture& other);
	

private:
	unsigned int textureID;
	bool transparency;
	std::string& filepath;
	std::string& texType;




	};
}



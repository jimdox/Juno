#pragma once

#include <GL/gl.h>
#include <string>
namespace bbx {
class Texture
{
public:
	Texture(const std::string& filepath);
	virtual ~Texture();

	unsigned int getID();
	float getReflectivity();
	float getShineDamper();
	void setReflectivity(float& r);
	void setShineDamper(float& sd);

	Texture(const Texture& other);
	Texture& operator=(const Texture& other);

protected:

private:
	unsigned int textureID;
	float shineDamper;
	float reflectivity;




	};
}



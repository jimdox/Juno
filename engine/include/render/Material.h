#pragma once
/* This is an example class of how a Material would work, in the future, I will add features to the shader compiler 
    to detect what variables are needed for a given material, instead of hard-coding them here. */
namespace juno 
{
class Material
{
public:
    Material();
    ~Material();
    
    inline unsigned int getAssocShaderID()
    {
        return shaderID;
    }

    inline glm::vec3& getBaseColor()
    {
        return baseColor;
    }

    inline float getReflectivity()
    {
        return reflectivity;
    }

    inline float getShineDamper()
    {
        return shineDamper;
    }

    void setBaseColor(const glm::vec3& baseColor);
    void setReflectivity(float reflectivity);
    void setShineDamper(float shineDamper);

private:
    unsigned int shaderID;
	glm::vec3 baseColor;
	float reflectivity;
	float shineDamper;
	// std::vector<Texture> diffuseTextures;
	// std::vector<Texture> specularTextures;
    //Texture normalMap;
    //std::vector<input vars>

};
}
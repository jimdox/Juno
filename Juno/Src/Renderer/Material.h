#pragma once
/* This is an example class of how a Material would work, in the future, I will add features to the shader compiler 
    to detect what variables are needed for a given material, instead of hard-coding them here. */
namespace Juno 
{
class Material
{
public:
    Material();
    ~Material();
    
    inline unsigned int GetAssocShaderID()
    {
        return shaderID;
    }

    inline glm::vec3& GetBaseColor()
    {
        return baseColor;
    }

    inline float GetReflectivity()
    {
        return reflectivity;
    }

    inline float GetShineDamper()
    {
        return shineDamper;
    }

    void SetBaseColor(const glm::vec3& baseColor);
    void SetReflectivity(float reflectivity);
    void SetShineDamper(float shineDamper);

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
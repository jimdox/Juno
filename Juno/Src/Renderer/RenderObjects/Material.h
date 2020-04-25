#pragma once
#include "Utils/Memory.h"
#include "Renderer/Data/Shader.h"
#include "Renderer/RenderObjects/Light.h"
/* This is an example class of how a Material would work, in the future, I will add features to the shader compiler 
    to detect what variables are needed for a given material, instead of hard-coding them here. */
namespace Juno 
{
class Material
{
public:
    Material();
    ~Material();

    Vec3& GetBaseColor();
    float GetReflectivity();
    float GetShineDamper();

    SPtr<Shader> GetShader();
    void CacheUniforms();
    void Bind();
    void Unbind();

    void SetBaseColor(const Vec3& baseColor);
    void SetReflectivity(float reflectivity);
    void SetShineDamper(float shineDamper);

    void LoadModelMatrix(const Mat4& modelMat);
    void LoadProjectionMatrix(const Mat4& projectionMat);
    void LoadViewMatrix(const Mat4& viewMat);
    void LoadLights(std::vector<SPtr<Light>>& lights);
    void LoadVarsToShader();


private:
    SPtr<Shader> shader;
	Vec3 baseColor;
	float reflectivity;
	float shineDamper;

    /* uniform caching: */
	static const unsigned int NUM_LIGHTS = 4;
	unsigned int loc_modelMatrix;
	unsigned int loc_projectionMatrix;
	unsigned int loc_viewMatrix;
	unsigned int loc_reflectivity;
	unsigned int loc_shineDamper;
	unsigned int loc_baseColor;

	unsigned int loc_lightPositions[NUM_LIGHTS];
	unsigned int loc_lightColors[NUM_LIGHTS];
	unsigned int loc_attenuations[NUM_LIGHTS];

	// std::vector<Texture> diffuseTextures;
	// std::vector<Texture> specularTextures;
    //Texture normalMap;
    //std::vector<input vars>

};
}
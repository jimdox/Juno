#include "pch.h"
#include "render/Material.h"
#include "core/AssetManager.h"
#include "render/Renderer.h"
using namespace juno;

Material::Material()
{
    shaderID = 0;
    baseColor = glm::vec3(0.5,0.5,0.5);
    reflectivity = 0.4f;
    shineDamper = 0.8f;
}

// Material::Material(Shader& shader)
// {
//     shaderID = 0;
//     baseColor = glm::vec3(0.5,0.5,0.5);
//     reflectivity = 0.4f;
//     shineDamper = 0.2f;
// }

Material::~Material()
{

}

void Material::setBaseColor(const glm::vec3& baseColor)
{
    this->baseColor = baseColor;
}

void Material::setReflectivity(float reflectivity)
{
    this->reflectivity = reflectivity;
}

void Material::setShineDamper(float shineDamper)
{
    this->shineDamper = shineDamper;
}


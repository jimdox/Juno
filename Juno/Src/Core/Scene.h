#pragma once
#include "Entities/Entity.h"
#include "Renderer/Light.h"
#include "Entities/SkyBox.h"
namespace Juno 
{
class Scene 
{
public:
    Scene();
    ~Scene();

    void Add(Entity& entity);
    void Add(Light& light);
    void Clear();

    std::vector<Entity>& GetEntities();
    std::vector<Light>& GetLights();
    //SkyBox& getSkyBox();

private:
    //SkyBox skybox;
    std::vector<Entity> entities;
    std::vector<Light> lights;

};
}
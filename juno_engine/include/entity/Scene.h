#pragma once
#include "entity/Entity.h"
#include "render/lights/Light.h"
#include "entity/SkyBox.h"
namespace juno {

class Scene 
{
public:
    Scene();
    ~Scene();

    void addEntity(Entity& entity);
    void addLight(Light& light);
    void clear();

    std::vector<Entity>& getEntities();
    std::vector<Light>& getLights();
    SkyBox& getSkyBox();

private:
    SkyBox skybox;
    std::vector<Entity> entities;
    std::vector<Light> lights;

};


}
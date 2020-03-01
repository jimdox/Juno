#include "entity/Scene.h"

using namespace juno;

Scene::Scene()
{

}

Scene::~Scene()
{
    clear();
}

void Scene::addEntity(Entity& entity)
{
    entities.push_back(entity);
}

void Scene::addLight(Light& light)
{
    lights.push_back(light);
}

void Scene::clear()
{
    entities.clear();
    lights.clear();
}

std::vector<Entity>& Scene::getEntities()
{
    return entities;
}

std::vector<Light>& Scene::getLights()
{
    return lights;
}


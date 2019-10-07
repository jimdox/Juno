#include "EntityManager.h"

EntityManager::EntityManger()
{
    entities(10);
}

EntityManager::~EntityManager()
{
    entities.clear();
}

void loadEntity(std::string& filepath)
{
    entities.push_back(new Entity(filepath));
}

Entity& getEntity(unsigned int n)
{
    if(entities[n])
    {
        return entities[n];
    }
    BBX_ERR(("Error, entity: "+ n + " not found.").c_str());
    return nullptr;
}
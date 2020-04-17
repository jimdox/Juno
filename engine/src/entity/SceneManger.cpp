#include "pch.h"
#include "entity/SceneManager.h"

using namespace juno;


Scene& SceneManager::getScene()
{
    return scene;
}

void SceneManager::setScene(Scene& scene)
{
    this->scene = scene;
}
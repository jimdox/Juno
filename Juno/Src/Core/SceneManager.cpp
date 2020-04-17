#include "jnpch.h"
#include "Core/SceneManager.h"

using namespace Juno;


Scene& SceneManager::GetScene()
{
    return scene;
}

void SceneManager::SetScene(Scene& scene)
{
    this->scene = scene;
}
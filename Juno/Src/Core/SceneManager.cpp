#include "jnpch.h"
#include "Core/SceneManager.h"

using namespace Juno;

SPtr<Scene> SceneManager::CreateScene()
{
    scene = std::make_shared<Scene>();
    return scene;
}


SPtr<Scene> SceneManager::GetScene()
{
    return scene;
}

void SceneManager::SetScene(SPtr<Scene> scene)
{
    this->scene = scene;
}


#pragma once
#include "Core/Scene.h"

namespace Juno
{
class SceneManager
{
friend class Dock;
public:
    static SceneManager& Get()
    {   
        static SceneManager s_instance;
        return s_instance;
    }
    SceneManager(const SceneManager& SceneManager) = delete;

    SPtr<Scene> CreateScene();
    SPtr<Scene> GetScene();
    void SetScene(SPtr<Scene> scene);

private:
    SceneManager(){}
    SPtr<Scene> scene;

};
}
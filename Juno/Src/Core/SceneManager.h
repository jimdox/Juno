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

    Scene& GetScene();
    void SetScene(Scene& scene);

private:
    SceneManager(){}
    Scene scene;

};
}
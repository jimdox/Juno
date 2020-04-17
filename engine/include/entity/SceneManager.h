#pragma once
#include "entity/Scene.h"

namespace juno
{
class SceneManager
{
friend class Dock;
public:
    static SceneManager& get()
    {   
        static SceneManager s_instance;
        return s_instance;
    }
    SceneManager(const SceneManager& SceneManager) = delete;

    Scene& getScene();
    void setScene(Scene& scene);



private:
    SceneManager(){}
    Scene scene;


};
}
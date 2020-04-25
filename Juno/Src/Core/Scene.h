#pragma once
#include "Utils/Memory.h"
#include "Renderer/RenderObjects/Camera.h"
#include "Renderer/RenderObjects/Light.h"
#include "Entities/Entity.h"
#include "Entities/SkyBox.h"

namespace Juno 
{
class Scene 
{
public:
    Scene();
    Scene(SPtr<Camera> camera);
    ~Scene();


    void CreateEntity(SPtr<Mesh> mesh, const Vec3& pos, const Vec3& rot, float scale, const std::string& name);
    void CreateLight(const Vec3& pos, const Vec3& color);
    
    void ClearScene();

    std::vector<SPtr<Entity>>& GetEntities();
    std::vector<SPtr<Light>>& GetLights();
    SPtr<Camera> GetCamera();
    //SkyBox& getSkyBox();

protected:
    void Add(SPtr<Entity> entity);
    void Add(SPtr<Light> light);

private:
    //SkyBox skybox;
    std::vector<SPtr<Entity>> entities;
    std::vector<SPtr<Light>> lights;
    SPtr<Camera> camera;
};
}
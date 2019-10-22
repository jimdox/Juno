#pragma once
#include "entity/Entity.h"
#include "Camera.h"
#include "Shader.h"
#include <tuple>

namespace bx {
class RenderQueue
{
public:
    RenderQueue();  //std::shared_ptr<Camera> & camera);
    ~RenderQueue();

    void setLights(std::vector<Light> &lights);
    void addLight(Light &light);

    void submit(Entity* entity, std::shared_ptr<Shader> shader);
    void remove(Entity& entity);
    void render(Camera* camera);
    void clear();

private:
    std::vector<Entity *> entities;
    std::vector<std::shared_ptr<Shader>> shaders;
    std::vector<Entity *> instanced_entities;
    std::vector<Light> lights;
    //std::shared_ptr<Camera> camera;
};
}
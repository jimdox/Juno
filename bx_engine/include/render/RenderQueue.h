#pragma once
#include "entity/Entity.h"
#include "Camera.h"
#include "Shader.h"
#include <map>

namespace bx {
class RenderQueue
{
public:
    RenderQueue(std::shared_ptr<Camera> camera);
    ~RenderQueue();

    void submit(std::shared_ptr<Entity> entity, std::shared_ptr<Shader> shader);
    void remove(std::shared_ptr<Entity> & entity);
    void render();
    void clear();

private:
    std::map< std::shared_ptr<Shader>, std::shared_ptr<Entity> > entities;

    std::shared_ptr<Camera> camera;
};
}
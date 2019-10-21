#include "render/RenderQueue.h"

using namespace bx;

RenderQueue::RenderQueue(std::shared_ptr<Camera> camera) : camera(camera)
{

}

RenderQueue::~RenderQueue()
{
    clear();
}

void RenderQueue::submit(std::shared_ptr<Entity> entity, std::shared_ptr<Shader> shader)
{
    //entities.insert(std::pair<std::shared_ptr<Shader>, std::shared_ptr<Entity>>(entity, shader));
}

void RenderQueue::remove(std::shared_ptr<Entity> & entity)
{
   // auto tmp = entities.extract(entity)
}

void RenderQueue::clear()
{
    entities.clear();
}


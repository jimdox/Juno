#pragma once
#include "Events/Events.h"


namespace Juno {

class RenderEvent : public Event
{
public:
    inline EventCategory GetCategory() const { return EventCategory::CAT_PROGRAM; }
};

class RenderWireFrameEvent : public RenderEvent
{
public:
    RenderWireFrameEvent(bool flag) : wireframe(flag) {}

    inline EventType GetType() const { return EventType::RENDER_POLYGON_WIREFRAME; }
    inline bool IsModeWireFrame() const { return wireframe; }

private:
    bool wireframe;
};

}
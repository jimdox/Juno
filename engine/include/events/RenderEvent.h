#pragma once
#include "events/Event.h"


namespace juno {

class RenderEvent : public Event
{
public:
    inline EventCategory getCategory() const { return EventCategory::CAT_PROGRAM; }
};

class RenderWireframeEvent : public RenderEvent
{
public:
    RenderWireframeEvent(bool flag) : wireframe(flag) {}

    inline EventType getType() const { return EventType::RENDER_POLYGON_WIREFRAME; }
    inline bool modeSetWireframe() const { return wireframe; }

private:
    bool wireframe;
};

}
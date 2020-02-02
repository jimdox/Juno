#pragma once

#include "events/Event.h"
#include "core/InputManager.h"

namespace juno {
class MouseButtonEvent : public Event
{
public:
    MouseButtonEvent(MouseCode b) { code = b; }
    inline MouseCode getMouseCode() const { return code; }
    inline EventCategory getCategory() const { return EVENT_CAT_INPUT; }

protected:
    MouseCode code;
};


class MousePressEvent : public MouseButtonEvent
{
public: 
    MousePressEvent(MouseCode b) : MouseButtonEvent(b){}

};


class MouseReleaseEvent : public MouseButtonEvent
{
public: 
    MouseReleaseEvent(MouseCode b) : MouseButtonEvent(b){} 
};


class MouseMoveEvent : public Event
{
public: 
    MouseMoveEvent(float dx, float dy) : x(dx), y(dy){}
    inline float getX() const { return x; }
    inline float getY() const { return y; }
    inline EventCategory getCategory() const { return EVENT_CAT_INPUT; }

protected:
    float x, y;
};

class MouseScrollEvent : public Event
{
public: 
    MouseScrollEvent(float xOffset, float yOffset) : x(xOffset), y(yOffset) {}
    inline float getXOffset() const { return x; }
    inline float getYOffset() const { return y; }
protected: 
    float x, y;
};

}
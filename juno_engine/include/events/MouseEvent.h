#pragma once

#include "events/Event.h"
#include "core/InputManager.h"

namespace juno {
class MouseButtonEvent : public Event
{
public:
    MouseButtonEvent(MouseCode b) { code = b; }
    inline MouseCode getMouseCode() const { return code; }
    inline EventCategory getCategory() const { return EVENT_CAT_MOUSE; }

protected:
    MouseCode code;
};


class MousePressEvent : public MouseButtonEvent
{
public: 
    MousePressEvent(MouseCode b) : MouseButtonEvent(b){}
    inline EventType getType() const { return MOUSE_BUTTON_PRESS; } 

};


class MouseReleaseEvent : public MouseButtonEvent
{
public: 
    MouseReleaseEvent(MouseCode b) : MouseButtonEvent(b){} 
    inline EventType getType() const { return MOUSE_BUTTON_RELEASE; } 

};


class MouseMoveEvent : public Event
{
public: 
    MouseMoveEvent(float dx, float dy) : x(dx), y(dy){}
    inline EventType getType() const { return MOUSE_MOVE; } 
    inline EventCategory getCategory() const { return EVENT_CAT_MOUSE; }
    inline float getX() const { return x; }
    inline float getY() const { return y; }


protected:
    float x, y;
};

class MouseScrollEvent : public Event
{
public: 
    MouseScrollEvent(float xOffset, float yOffset) : x(xOffset), y(yOffset) {}
    inline EventType getType() const { return MOUSE_SCROLL; } 
    inline EventCategory getCategory() const { return EVENT_CAT_MOUSE; }

    inline float getXOffset() const { return x; }
    inline float getYOffset() const { return y; }
protected: 
    float x, y;
};

}
#pragma once
#include "Events/Events.h"

namespace Juno {

enum MouseCode {
    M_BUTTON_LEFT = 0,
    M_BUTTON_RIGHT = 1,
    M_BUTTON_MID = 2
};

class MouseEvent : public Event
{
public: 
    inline EventCategory GetCategory() const { return EventCategory::CAT_MOUSE; }
};


class MouseButtonEvent : public MouseEvent
{
public:
    MouseButtonEvent(MouseCode b) { code = b; }
    inline MouseCode GetMouseCode() const { return code; }
    inline EventCategory GetCategory() const { return EventCategory::CAT_MOUSE_BUTTON; }

protected:
    MouseCode code;
};


class MousePressEvent : public MouseButtonEvent
{
public: 
    MousePressEvent(MouseCode b) : MouseButtonEvent(b){}
    inline EventType GetType() const { return EventType::MOUSE_BUTTON_PRESS; } 

};


class MouseReleaseEvent : public MouseButtonEvent
{
public: 
    MouseReleaseEvent(MouseCode b) : MouseButtonEvent(b){} 
    inline EventType GetType() const { return EventType::MOUSE_BUTTON_RELEASE; } 

};


class MouseMoveEvent : public MouseEvent
{
public: 
    MouseMoveEvent(float dx, float dy) : x(dx), y(dy){}
    inline EventType GetType() const { return EventType::MOUSE_MOVE; } 
    inline float GetX() const { return x; }
    inline float GetY() const { return y; }


protected:
    float x, y;
};

class MouseScrollEvent : public MouseEvent
{
public: 
    MouseScrollEvent(float xOffset, float yOffset) : x(xOffset), y(yOffset) {}
    inline EventType GetType() const { return EventType::MOUSE_SCROLL; } 

    inline float GetXOffset() const { return x; }
    inline float GetYOffset() const { return y; }
    
protected: 
    float x, y;
};

class MouseEventDispatcher : public EventDispatcher
{
public:
    ~MouseEventDispatcher(){}
};

}
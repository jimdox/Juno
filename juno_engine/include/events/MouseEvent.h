#pragma once
#include "events/Event.h"

namespace juno {

enum class MouseCode {
    M_BUTTON_LEFT,
    M_BUTTON_MID,
    M_BUTTON_RIGHT
};

class MouseEvent : public Event
{
public: 
    inline EventCategory getCategory() const { return EventCategory::CAT_MOUSE; }
};


class MouseButtonEvent : public MouseEvent
{
public:
    MouseButtonEvent(MouseCode b) { code = b; }
    inline MouseCode getMouseCode() const { return code; }
    inline EventCategory getCategory() const { return EventCategory::CAT_MOUSE_BUTTON; }

protected:
    MouseCode code;
};


class MousePressEvent : public MouseButtonEvent
{
public: 
    MousePressEvent(MouseCode b) : MouseButtonEvent(b){}
    inline EventType getType() const { return EventType::MOUSE_BUTTON_PRESS; } 

};


class MouseReleaseEvent : public MouseButtonEvent
{
public: 
    MouseReleaseEvent(MouseCode b) : MouseButtonEvent(b){} 
    inline EventType getType() const { return EventType::MOUSE_BUTTON_RELEASE; } 

};


class MouseMoveEvent : public MouseEvent
{
public: 
    MouseMoveEvent(float dx, float dy) : x(dx), y(dy){}
    inline EventType getType() const { return EventType::MOUSE_MOVE; } 
    inline float getX() const { return x; }
    inline float getY() const { return y; }


protected:
    float x, y;
};

class MouseScrollEvent : public MouseEvent
{
public: 
    MouseScrollEvent(float xOffset, float yOffset) : x(xOffset), y(yOffset) {}
    inline EventType getType() const { return EventType::MOUSE_SCROLL; } 

    inline float getXOffset() const { return x; }
    inline float getYOffset() const { return y; }
    
protected: 
    float x, y;
};

class MouseEventDispatcher : public EventDispatcher
{
public:
    ~MouseEventDispatcher(){}
};

}
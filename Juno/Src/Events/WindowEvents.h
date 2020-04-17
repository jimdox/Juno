#pragma once
#include "Events/Events.h"


namespace Juno {
class WindowEvent : public Event 
{
public:
    inline EventCategory GetCategory() const { return EventCategory::CAT_PROGRAM; }
};

class WindowResizeEvent : public WindowEvent
{
public:
    WindowResizeEvent(int sWidth, int sHeight) : width(sWidth), height(sHeight) {}

    inline EventType GetType() const { return EventType::WINDOW_RESIZE; }

    inline float GetScreenWidth() const { return width; }
    inline float GetScreenHeight() const { return height; }

private:
    float width, height;
};

class WindowCloseEvent : public WindowEvent
{
public:
    inline EventType GetType() const { return EventType::WINDOW_CLOSE; }
};

class CustomWindowEvent : public WindowEvent
{
public:
    CustomWindowEvent(int code) : sceneCode(code){}
    inline EventType GetType() const {return EventType::SCENE_SWITCH; }
    int sceneCode;
};

class WindowEventDispatcher : public EventDispatcher
{
public:
    WindowEventDispatcher(){}
    ~WindowEventDispatcher(){}
};


}


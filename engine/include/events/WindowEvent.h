#pragma once
#include "events/Event.h"


namespace juno {
class WindowEvent : public Event 
{
public:
    inline EventCategory getCategory() const { return EventCategory::CAT_PROGRAM; }
};

class WindowResizeEvent : public WindowEvent
{
public:
    WindowResizeEvent(int s_width, int s_height) : width(s_width), height(s_height) {}

    inline EventType getType() const { return EventType::WINDOW_RESIZE; }

    inline float getScreenWidth() const { return width; }
    inline float getScreenHeight() const { return height; }

private:
    float width, height;
};

class WindowCloseEvent : public WindowEvent
{
public:
    inline EventType getType() const { return EventType::WINDOW_CLOSE; }
};

class CustomWindowEvent : public WindowEvent
{
public:
    CustomWindowEvent(int code) : sceneCode(code){}
    inline EventType getType() const {return EventType::SCENE_SWITCH; }
    int sceneCode;
};

class WindowEventDispatcher : public EventDispatcher
{
public:
    WindowEventDispatcher(){}
    ~WindowEventDispatcher(){}
};


}


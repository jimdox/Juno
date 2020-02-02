#pragma once
#include "pch.h"
/**/
namespace juno {
enum EventType : short
{
    NONE, WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_NOT_FOCUS, WINDOW_MOVE,
    PROG_UPDATE, PROG_RENDER, 
    KEY_PRESS, KEY_RELEASE, 
    MOUSE_BUTTON_PRESS, MOUSE_BUTTON_RELEASE, MOUSE_MOVE, MOUSE_SCROLL
};

enum EventCategory: short
{
    NONE = 0,
    EVENT_CAT_PROGRAM      = 1 << 0,
    EVENT_CAT_INPUT        = 1 << 1, 
    EVENT_CAT_KEYBOARD     = 1 << 2,
    EVENT_CAT_MOUSE        = 1 << 3,
    EVENT_CAT_MOUSE_BUTTON = 1 << 4
};


class Event
{
public:
    virtual ~Event();
    virtual EventType getType() const = 0;
    virtual EventCategory getCategory() const = 0;

    inline bool inCategory(EventCategory cat)
    {
        return cat == getCategory();
    }
    
    EventCategory category;
    bool handled = false;
};


class EventDispatcher
{
friend class EventListener;
public:
    virtual ~EventDispatcher();
    void addListener(EventListener* listener);
    void rmListener(EventListener* listener);

protected:
    void notify(Event& e)
    {
        for(unsigned int i = 0; i < numListeners; i++)
        {
            eListeners[i]->onNotify(e);
        }
    }

private:
    EventListener* eListeners[5];
    unsigned int numListeners;
};


class EventListener 
{
public:
    virtual ~EventListener();
    virtual void onNotify(Event& e) = 0;


};  
}
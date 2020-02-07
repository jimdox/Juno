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
    OTHER,
    EVENT_CAT_PROGRAM,
    EVENT_CAT_KEYBOARD,
    EVENT_CAT_MOUSE,
    EVENT_CAT_MOUSE_BUTTON
};


class Event
{
public:
    virtual ~Event();
    inline virtual EventType getType() const = 0;
    virtual EventCategory getCategory() const = 0;

    inline bool inCategory(EventCategory cat)
    {
        return cat == getCategory();
    }
    
    EventCategory category;
    bool handled = false;
};


class EventListener 
{
public:
    virtual ~EventListener();
    virtual void onNotify(const Event& e) = 0;


};  


class EventDispatcher
{
friend class EventListener;
public:
    virtual ~EventDispatcher();
    void addListener(EventListener* listener);
    void rmListener(EventListener* listener);

    void notify(const Event& e)
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


}
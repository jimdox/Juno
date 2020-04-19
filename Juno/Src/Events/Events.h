#pragma once

/* Event System uses the Dispatcher and Listener pattern, also known as 'Observers'. 
   The implementation is currently blocking, meaning that all events must be handled
   as they are recieved, however it is planned that not-urgent events be put
   into an Event Queue for later processing. */

namespace Juno {

enum class EventType : short
{
    NONE, WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_NOT_FOCUS, WINDOW_MOVE,
    PROG_UPDATE, PROG_RENDER, RENDER_POLYGON_WIREFRAME,
    SCENE_SWITCH,
    KEY_PRESS, KEY_RELEASE, 
    MOUSE_BUTTON_PRESS, MOUSE_BUTTON_RELEASE, MOUSE_MOVE, MOUSE_SCROLL
};

enum class EventCategory: short
{
    NONE,
    CAT_PROGRAM,
    CAT_KEYBOARD,
    CAT_MOUSE,
    CAT_MOUSE_BUTTON
};

/* abstract definition for an event */
class Event
{
public:
    virtual ~Event(){}
    virtual EventType GetType() const = 0;
    virtual EventCategory GetCategory() const = 0;

    inline bool InCategory(EventCategory cat)
    {
        return cat == GetCategory();
    }
    
    EventCategory category;
    bool handled = false;
};


class EventListener 
{
public:
    virtual ~EventListener(){}                   /* Listeners should always be removed */
    virtual void OnEvent(const Event& e) = 0;      /* from dispatchers upon destruction */
    virtual void OnAttach() = 0;

};  


class EventDispatcher
{
friend class EventListener;
public:
    virtual ~EventDispatcher(){}
    void AddListener(EventListener* listener)
    {
        if(numListeners < MAX_EVENT_LISTENERS)
        {
            listeners[numListeners++] = listener;
            listener->OnAttach();
        }
    }
    void RmListener(EventListener* listener)
    {
        for(short i = 0; i < numListeners; i++)
        {
            if(listeners[i] == listener)
                listeners[i] = nullptr;
        }
    }

    void Notify(const Event& e)
    {
        for(int i = 0; i < numListeners; i++)
        {
            listeners[i]->OnEvent(e);
        }
    }
protected:    
    static const unsigned int MAX_EVENT_LISTENERS = 5;
    short numListeners = 0;
    EventListener* listeners[MAX_EVENT_LISTENERS];
};
}
#pragma once
#include "pch.h"

namespace bx {
enum EventType : short
{
    NONE, WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_NON_FOCUS, WINDOW_MOVE,
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

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
                                                                virtual EventType getEventType() const override { return getStaticType(); }\
                                                                virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CAT(category) virtual int getCatFlags() const override { return category; }

class Event 
{
public:
    bool handled = false;

    virtual EventType getType() const = 0;
    virtual const char* getName() const = 0;
    virtual unsigned int getCatFlags() = 0;
    virtual std::string toString() const { return getName(); }

    inline bool inCategory(EventCategory cat)
    {
        return getCatFlags() & cat;
    }
};

class EventDispatcher 
{
public: 
    EventDispatcher(Event& e) : event(e){}

    template<typename t, typename f>

    bool dispatch(const f& func)    
    {
        if(event.getType() == t::getStaticType())
        {
            event.handled = func(static_cast<t&>(event));
            return true;
        }
        return false;
    }

private: 
    Event& event;
};  

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
    return os << e.toString();
}
}
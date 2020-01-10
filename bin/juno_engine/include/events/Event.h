#pragma once
#include "pch.h"
namespace juno {
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


class Event 
{
friend class EventObserver;
public:
    virtual ~Event();
    virtual EventType type() const = 0;
    
    bool handled = false;

};

class EventObserver 
{
// public:
//     using SlotType = std::function<void( const Event&)>;
//     void addEventCategory(const Event);
// private: 
public:
    virtual ~EventObserver();
    virtual void onNotify() = 0;

};  

// inline std::ostream& operator<<(std::ostream& os, const Event& e)
// {
//     return os << e.toString();
// }
}
#pragma once
#include "Events/Events.h"

namespace Juno {

class KeyEvent : public Event
{
public:
    inline int GetKeyCode() const { return keyCode; }
    inline EventCategory GetCategory() const { return EventCategory::CAT_KEYBOARD; }

protected:
    KeyEvent(int keyCode) : keyCode(keyCode){}
    int keyCode;
};


class KeyPressEvent : public KeyEvent  
{
public: 
    KeyPressEvent(int keyCode, int repeats) : KeyEvent(keyCode), repeats(repeats){}

    inline EventType GetType() const { return EventType::KEY_PRESS; }
    inline int GetRepeats() const { return repeats; }

protected:
    int repeats;
};


class KeyReleaseEvent : public KeyEvent
{
public:
    KeyReleaseEvent(int keyCode) : KeyEvent(keyCode){}
    inline EventType GetType() const { return EventType::KEY_RELEASE; }
};


class KeyEventListener : public EventListener
{
public:
    ~KeyEventListener(){}
    virtual void OnEvent(const KeyEvent& e) = 0;

};

class KeyEventDispatcher : public EventDispatcher
{
public:
    KeyEventDispatcher(){}
    ~KeyEventDispatcher(){}
    
};

}
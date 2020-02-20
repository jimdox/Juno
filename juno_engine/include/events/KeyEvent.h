#pragma once
#include "events/Event.h"
#include <GLFW/glfw3.h>

namespace juno {

class KeyEvent : public Event
{
public:
    inline int getKeyCode() const { return keyCode; }
    inline EventCategory getCategory() const { return EventCategory::CAT_KEYBOARD; }

protected:
    KeyEvent(int key_code) : keyCode(key_code){}
    int keyCode;
};


class KeyPressEvent : public KeyEvent  
{
public: 
    KeyPressEvent(int key_code, int repeats) : KeyEvent(key_code), repeats(repeats){}

    inline EventType getType() const { return EventType::KEY_PRESS; }
    inline int getRepeats() const { return repeats; }

protected:
    int repeats;
};


class KeyReleaseEvent : public KeyEvent
{
public:
    KeyReleaseEvent(int keyCode) : KeyEvent(keyCode){}
    inline EventType getType() const { return EventType::KEY_RELEASE; }
};


class KeyEventListener : public EventListener
{
public:
    ~KeyEventListener(){}
    virtual void onEvent(const KeyEvent& e) = 0;

};

class KeyEventDispatcher : public EventDispatcher
{
public:
    KeyEventDispatcher(){}
    ~KeyEventDispatcher(){}
    
};

}
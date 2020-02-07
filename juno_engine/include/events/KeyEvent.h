#pragma once
#include "events/Event.h"
namespace juno {

class KeyEvent : public Event
{
public:
    inline int getKeyCode() const { return keyCode; }
    inline EventCategory getCategory() const { return EVENT_CAT_KEYBOARD; }

protected:
    KeyEvent(int key_code) : keyCode(key_code){}
    int keyCode;
};


class KeyPressEvent : public KeyEvent  
{
public: 
    KeyPressEvent(int key_code, int repeats) : KeyEvent(key_code), repeats(repeats){}

    inline int getRepeats() const { return repeats; }

protected:
    int repeats;
};


class KeyReleaseEvent : public KeyEvent
{
public:
    KeyReleaseEvent(int keyCode) : KeyEvent(keyCode){}

};


class KeyEventListener
{
public:
    ~KeyEventListener(){}
    void onNotify(const KeyEvent& e)
    {

    }
};

class KeyEventDispatcher
{
public:
    KeyEventDispatcher()
    {

    }

};

}
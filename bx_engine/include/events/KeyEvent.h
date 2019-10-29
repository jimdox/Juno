#pragma once
#include "events/Event.h"
namespace bx {

class KeyEvent : public Event
{
public:
    inline int getKeyCode() const { return key_code; }


protected:
    KeyEvent(int key_code) : key_code(key_code){}
    int key_code;
};

class KeyPressedEvent : public KeyEvent  
{
public: 
    KeyPressedEvent(int key_code, int repeats) : KeyEvent(key_code), repeats(repeats){}

    inline int getRepeats() const { return repeats; }

    std::string toString() const override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << key_code << " [" << repeats << " repeats]";
        return ss.str();
    }
       
protected:
    int repeats;
};
}
#pragma once
#include "events/Event.h"


namespace juno {
class WindowEvent : public Event 
{
public:
    inline EventCategory getCategory() const { return EventCategory::CAT_PROGRAM; }
};

class WindowCloseEvent : public WindowEvent
{
public:
    inline EventType getType() const { return EventType::WINDOW_CLOSE; }
};

}
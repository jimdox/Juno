#pragma once
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"
namespace Juno {

/* can store a queue of events for blocking, or filter events between listeners */
class EventBus
{
public:
    EventBus(){}

protected:

};


class IOEventBus : public EventBus
{
public:


};
}
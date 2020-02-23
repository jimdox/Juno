#pragma once
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "WindowEvent.h"
namespace juno {

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
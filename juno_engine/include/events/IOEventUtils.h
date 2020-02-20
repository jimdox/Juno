#pragma once
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/WindowEvent.h"
/* alternative to using onEvent for everything that might ever need to access keyboard states */
namespace juno {

// class IOEvent 
// {
// public:
//     IOCallback(const IOCallback&) = delete;

//     static IOCallback& getInstance()
//     {
//         return s_instance;
//     }

//     float mouseDX()
//     {
//         float delta_x = mouse_dx;
//         mouse_dx = 0;
//         return delta_x;
//     }

//     float mouseDY()
//     {
//         float delta_y = mouse_dy;
//         mouse_dy = 0;
//         return delta_y;
//     }

//     bool isKeyDown(unsigned int k)
//     {
//         return keys[k];
//     }

//     // bool mouseButtonDown(MouseCode& mc)
//     // {

//     // }

//     void onEvent(const Event &e)
//     {
//     }



// private:
//     IOCallback()
//     {
        
//     }
//     static IOCallback s_instance;
//     bool keys[93];
     

    
// };
/* stores changes due to events recieved for later use by Camera.h, etc. */
typedef struct IOEventContainer
{
    bool keys[93];
    float mouse_dx = 0.0f;
    float mouse_dy = 0.0f;
    float prev_mouse_x = 0.0f;
    float prev_mouse_y = 0.0f;
    float mouse_dscroll = 0.0f;
    float prev_mouse_scroll = 0.0f;
    
    
    
    
    void setKeyStatus(int keycode, bool f_pressed)
    {
        if(keycode > 90)
        {   /* discard all keys above 90 for now */
            return;
        }
        keys[keycode] = f_pressed;
    }
    
    inline bool isKeyDown(int keycode)
    {
        return keys[keycode];
    }
    inline bool isMButtonDown(const MouseCode& mc)
    {
        if(mc == MouseCode::M_BUTTON_LEFT)
        {
            return keys[90];
        } else if (mc == MouseCode::M_BUTTON_MID) {
            return keys[91];
        } else {
            return keys[92];
        }
    }
    inline float mouseDX()
    {
        float dx = mouse_dx;
        mouse_dx = 0;
        return dx;
    }
    
    inline float mouseDY()
    {
        float dy = mouse_dy;
        mouse_dy = 0;
        return dy;
    }

    inline float mouseDScroll()
    {

    }

    inline void updateMousePosition(float x, float y)
    {
        mouse_dx += (x - prev_mouse_x);
        mouse_dy += (y - prev_mouse_y);
        prev_mouse_x = x;
        prev_mouse_y = y;
    }


};


}
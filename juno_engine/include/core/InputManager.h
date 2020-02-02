// #pragma once
// #include "pch.h"
// #include "core/KeyMap.h"
// #include "core/MouseMap.h"

namespace juno {

enum MouseCode : short
{
    BUTTON_0 = 0,
    BUTTON_1 = 1,
    BUTTON_2 = 2, 
    BUTTON_3 = 3,
    BUTTON_4 = 4,

    BUTTON_LEFT = BUTTON_0,
    BUTTON_MID = BUTTON_2,
    BUTTON_RIGHT = BUTTON_1
};





// class InputManager
// {
// public:
//     InputManager();
//     ~InputManager();

//     void setInputRecieved(bool flag);
//     bool inputRecieved();

//     double CURSOR_X;
//     double CURSOR_Y;
//     double CURSOR_DX;
//     double CURSOR_DY;

//     bool LMB_PRESSED;
//     bool RMB_PRESSED;

//     double SCROLL_Y;    /* normal mouse wheel */
//     double SCROLL_DY;  
//     double SCROLL_X;
//     double SCROLL_DX;


// private:
//     bool mouse_input_recieved = false;



// }
}
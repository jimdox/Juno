// #ifndef INPUT_STATES
// #define INPUT_STATES
#pragma once
namespace juno {
    
typedef struct MouseInputData {

bool MOUSE_DATA_RECIEVED;
bool LM_BUTTON_PRESS;
bool LM_BUTTON_REPEAT;
bool RM_BUTTON_PRESS;
bool RM_BUTTON_REPEAT;
double CURSOR_X; 
double CURSOR_Y; 
double CURSOR_DX;
double CURSOR_DY;
double SCROLL_X; 
double SCROLL_Y; 
double SCROLL_DX;
double SCROLL_DY;

} MouseInputData;

}
//#endif
#pragma once 
#include "pch.h"
#include "core/Log.h"
namespace bx{

static bool MOUSE_INPUT_RECIEVED = false;

static double CURSOR_X = 0.0;
static double CURSOR_Y = 0.0;
static double CURSOR_DX = 0.0;
static double CURSOR_DY = 0.0;

static bool LMB_PRESSED = false;
static bool RMB_PRESSED = false;

static double SCROLL_Y = 0.0;   /* normal mouse wheel */
static double SCROLL_DY = 0.0;  
static double SCROLL_X = 0.0;
static double SCROLL_DX = 0.0;  /* touchpad */


static void mousePositionHandler(GLFWwindow* window, double x_pos, double y_pos)
{
    if(!MOUSE_INPUT_RECIEVED)
    {
        CURSOR_DX += x_pos - CURSOR_X;
        CURSOR_DY += y_pos - CURSOR_Y;
    } else {
        CURSOR_DX += x_pos - CURSOR_X;
        CURSOR_DY += y_pos - CURSOR_Y;
        MOUSE_INPUT_RECIEVED = false;
    }
    CURSOR_X = x_pos;
    CURSOR_Y = y_pos;
}

static void mouseButtonMap(GLFWwindow* window, int button, int action, int mods)
{

}

static void mouseScrollHandler(GLFWwindow* window, double x_offset, double y_offset)
{
    if(!MOUSE_INPUT_RECIEVED)
    {
        SCROLL_DX += SCROLL_X - x_offset;
        SCROLL_DY += SCROLL_Y - y_offset;
    } else {
        SCROLL_DX = SCROLL_X - x_offset;
        SCROLL_DY = SCROLL_Y - y_offset;
        MOUSE_INPUT_RECIEVED = false;
    }
    SCROLL_X = x_offset;
    SCROLL_Y = y_offset;
}
}
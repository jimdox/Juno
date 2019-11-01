#pragma once
#include "core/Log.h"

static bool key_pressed_W = false;
static bool key_pressed_S = false;
static bool key_pressed_A = false;
static bool key_pressed_D = false;
static bool key_pressed_X = false;
static bool key_pressed_Q = false;
static bool key_pressed_E = false;

static bool key_pressed_RIGHT = false;
static bool key_pressed_LEFT = false;
static bool key_pressed_UP = false;
static bool key_pressed_DOWN = false;



static inline void kbdLayout(GLFWwindow* window, int key, int scancode, int action, int mods)
{	
	switch(key)
	{
		case GLFW_KEY_ESCAPE:
			if(action == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			break;
		case GLFW_KEY_W:
			if(action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				key_pressed_W = true;
				BX_WARN("W key pressed! ");
			} else {
				key_pressed_W = false;
			}
			break;
		case GLFW_KEY_S:
			if(action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				key_pressed_S = true;
			} else {
				key_pressed_S = false;
			}
			break;
		case GLFW_KEY_A:
			if(action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				key_pressed_A = true;
			} else {
				key_pressed_A = false;
			}
			break;
		case GLFW_KEY_D:
			if(action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				key_pressed_D = true;
			} else {
				key_pressed_D = false;
			}
			break;
		case GLFW_KEY_Q:
			if(action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				key_pressed_Q = true;
			} else {
				key_pressed_Q = false;
			}
			break;
		case GLFW_KEY_E:
			if(action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				key_pressed_E = true;
			} else {
				key_pressed_E = false;
			}
			break;
		case GLFW_KEY_X:
			if(action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				key_pressed_X = true;
			} else {
				key_pressed_X = false;
			}
			break;
		case GLFW_KEY_RIGHT:
			if(action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				key_pressed_RIGHT = true;
			} else {	
				key_pressed_RIGHT = false;
			}
			break;
		case GLFW_KEY_LEFT:
			if(action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				key_pressed_LEFT = true;
			} else {	
				key_pressed_LEFT = false;
			}
			break;
		case GLFW_KEY_UP:
			if(action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				key_pressed_UP = true;
			} else {	
				key_pressed_UP = false;
			}
			break;
		case GLFW_KEY_DOWN:
			if(action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				key_pressed_DOWN = true;
			} else {	
				key_pressed_DOWN = false;
			}
			break;
			
	
	}

}
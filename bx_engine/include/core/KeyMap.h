#pragma once
#include "core/Log.h"
static const float MOVE_SPEED = 0.2f;
static float s_cam_vx = 0.0f;
static float s_cam_vy = 0.0f;
static float s_cam_vz = 0.0f;
static float s_cam_roll = 0.0f;
static float s_cam_pitch = 0.0f;
static float s_cam_yaw = 0.0f;
static float s_deltaZoom = 0.0f;

static bool key_pressed_W = false;
static bool key_pressed_S = false;
static bool key_pressed_A = false;
static bool key_pressed_D = false;
static bool key_pressed_X = false;
static bool cam_mode_xray = false;

static void kbdLayout(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if(key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		key_pressed_W = true;
		BX_WARN("key pressed!");
	}
	else if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		key_pressed_S = true;
		BX_WARN("key pressed!");
	}

	if(key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		key_pressed_A;
		BX_WARN("key pressed!");
	}
	else if(key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		key_pressed_D;
		BX_WARN("key pressed!");
	}

	if(key == GLFW_KEY_B && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		s_cam_vy = -MOVE_SPEED;
	}
	else if(key == GLFW_KEY_G && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		s_cam_vy = MOVE_SPEED;
	}

	if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		s_cam_pitch = -MOVE_SPEED;
	}
	else if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		s_cam_pitch = MOVE_SPEED;
	}

	if(key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		s_cam_yaw = -MOVE_SPEED;
	}
	else if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		s_cam_yaw = MOVE_SPEED;

	}

	if(key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		s_cam_roll = -MOVE_SPEED;

	}
	else if(key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		s_cam_roll = MOVE_SPEED;
	}

	if(key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		key_pressed_X = true;
	}

}
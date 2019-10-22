#pragma once
#include "core/Log.h"
static const float MOVE_SPEED = 0.04f;
static float s_cam_vx = 0.0f;
static float s_cam_vy = 0.0f;
static float s_cam_vz = 0.0f;
static float s_cam_roll = 0.0f;
static float s_cam_pitch = 0.0f;
static float s_cam_yaw = 0.0f;
static float s_deltaZoom = 0.0f;

static void kbdLayout(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if(key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		s_cam_vz -= MOVE_SPEED;
		BX_WARN("key pressed!");
	}
	else if(key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		s_cam_vz += MOVE_SPEED;
		BX_WARN("key pressed!");
	}

	if(key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		s_cam_vx -= MOVE_SPEED;
		BX_WARN("key pressed!");
	}
	else if(key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		s_cam_vx += MOVE_SPEED;
		BX_WARN("key pressed!");
	}

	if(key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		s_cam_vy += MOVE_SPEED;
	}
	else if(key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		s_cam_vy -= MOVE_SPEED;
	}
	
	
	


}
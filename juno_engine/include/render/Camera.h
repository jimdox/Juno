#pragma once
#include "pch.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/WindowEvent.h"
#include "core/Log.h"
namespace juno { 

struct camera_io_states
{
	bool keys[91] = {false};
	bool mouse_buttons[3] = {false};

	float delta_mouse_x;
	float delta_mouse_y;
	float prev_mouse_x;
	float prev_mouse_y;

	bool isKeyDown(int keycode)
	{
		return keys[keycode];
	}

	bool isMButtonDown(MouseCode mc)
	{
	 	return mouse_buttons[((int) mc)];
	}	

	void setKeyStatus(int keycode, bool flag)
	{
		keys[keycode] = flag;
	}

	void setMouseButtonStatus(MouseCode mc, bool flag)
	{
		mouse_buttons[((int)mc)] = flag;
	}

	void mouseMoved(float x, float y)
	{
		if(mouse_buttons[0])
		{
			delta_mouse_x += (x - prev_mouse_x);
			delta_mouse_y += (y - prev_mouse_y);

		}
			prev_mouse_y = y;
			prev_mouse_x = x;
	}

	float getMouseDX()
	{
		float dx = delta_mouse_x;
		delta_mouse_x = 0;
		return dx;
	}

	float getMouseDY()
	{
		float dy = delta_mouse_y;
		delta_mouse_y = 0;
		return dy;
	}

};

class Camera : public EventListener, public EventDispatcher
{
public:
	//Camera(glm::vec3& pos);
    Camera(glm::vec3 pos, float yaw, float pitch, float roll);
	~Camera() override;

	/* Event stuff */
	void onAttach();
	void onEvent(const Event &e);

	void keyEventRecieved(int keycode, bool flag);

	void move(glm::vec3& dv, glm::vec3& dRot);
	void update();
	
	glm::vec3& getPosition();
	inline float getZoom()  { return zoom;  }
	inline float getRoll()  { return roll;  }
	inline float getPitch() { return pitch; }
	inline float getYaw()   { return yaw;   }

	void setPivot(glm::vec3& pivot);
	void setZoom(float z);
	void setPitch(float p);
	void setYaw(float y);
	
	void calculateCameraPos();
	glm::mat4& getProjectionMatrix();
	glm::mat4& resetProjectionMatrix();
	glm::mat4& getViewMatrix();

	const float DEFAULT_MOVE_SPEED = 6.5;
	const float DEFAULT_ROT_SPEED = 80;
	const double MIN_ZOOM = 52;
	const double MAX_ZOOM = -52;

private:
	void generateProjectionMatrix();

	glm::vec3 position;
	glm::vec3 velocity;

	/* --- */
	glm::vec3 delta_pos;
	glm::vec3 delta_rot;
	camera_io_states io_states;
	/* --- */

	glm::vec3 pivot;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	float yaw;
	float pitch;
	float roll;
	float zoom;
	float distanceToPivot;
	float angle_around_pivot;
	float moveSpeed;
	float cam_aspect_ratio = 1920.0f/1080.0f;


	const float FOV = 60.0f;				/* 60 -> ~80 is recommended */
	const float NEAR_PLANE = 0.1f;			/* min clipping dist. */
	const float FAR_PLANE = 1000.0f;		/* max clipping dist. */

};
}
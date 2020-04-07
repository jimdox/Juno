#pragma once
#include "pch.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/WindowEvent.h"
#include "core/Log.h"
namespace juno { 

struct mouse_states
{
	bool mouse_buttons[3] = {false, false, false};
	bool middle = false;
	float delta_mouse_x = 0.0f;
	float delta_mouse_y = 0.0f;
	float prev_mouse_x = 0.0f;
	float prev_mouse_y = 0.0f;
	float delta_scroll = 0.0f;

	bool isButtonDown(MouseCode mc)
	{
	 	return mouse_buttons[mc];
	}	
	
	void setButton(MouseCode mc, bool flag)
	{
		mouse_buttons[mc] = flag;
	}

	void setPosition(float x, float y)
	{
		if(mouse_buttons[0] || mouse_buttons[1] || mouse_buttons[2])
		{
			delta_mouse_x += (x - prev_mouse_x);
			delta_mouse_y += (y - prev_mouse_y);

		}
			prev_mouse_y = y;
			prev_mouse_x = x;
	}
	
	float getDX()
	{
		float dx = delta_mouse_x;
		delta_mouse_x = 0;
		return dx;
	}

	float getDY()
	{
		float dy = delta_mouse_y;
		delta_mouse_y = 0;
		return dy;
	}

	void setDScroll(float scroll_y)
	{
		delta_scroll += scroll_y;
	}

	float getDScroll()
	{
		float dy = delta_scroll;
		delta_scroll = 0;
		return dy;
	}
};

struct keyboard_states {
	bool keys[91];
	void init()
	{
		for(unsigned int i = 0; i < 91; i++)
		{
			keys[i] = false;
		}
	}
	bool isKeyDown(int keycode)
	{
		return keys[keycode];
	}

	void setKeyStatus(int keycode, bool flag)
	{
		keys[keycode] = flag;
	}
};


class Camera : public EventListener, public EventDispatcher
{
public:
	//Camera(glm::vec3& pos);
    Camera(glm::vec3 pos, glm::vec3 rot);
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
	
	void calculatePosition();
	glm::mat4& getProjectionMatrix();
	glm::mat4& resetProjectionMatrix();
	glm::mat4& getViewMatrix();

	const float DEFAULT_MOVE_SPEED = 6.5;
	const float DEFAULT_ROT_SPEED = 80;
	const double MIN_ZOOM = 22;
	const double MAX_ZOOM = -22;

private:
	void generateProjectionMatrix();

	mouse_states mouse;
	keyboard_states keyboard;

	float theta_around_pivot;
	float distance_to_pivot;
	glm::vec3 position;
	glm::vec3 velocity;

	/* --- */
	glm::vec3 delta_pos;
	glm::vec3 delta_rot;
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
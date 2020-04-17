#pragma once
#include "jnpch.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"
#include "Core/Log.h"
namespace Juno { 

struct MouseStates
{
	bool mouseButtons[3] = {false, false, false};
	bool middle = false;
	float mouseDX = 0.0f;
	float mouseDY = 0.0f;
	float prevMouseDX = 0.0f;
	float prevMouseDY = 0.0f;
	float deltaScroll = 0.0f;

	bool IsButtonDown(MouseCode mc)
	{
	 	return mouseButtons[mc];
	}	
	
	void SetButton(MouseCode mc, bool flag)
	{
		mouseButtons[mc] = flag;
	}

	void SetPosition(float x, float y)
	{
		if(mouseButtons[0] || mouseButtons[1] || mouseButtons[2])
		{
			mouseDX += (x - prevMouseDX);
			mouseDY += (y - prevMouseDY);

		}
			prevMouseDY = y;
			prevMouseDX = x;
	}
	
	float GetDX()
	{
		float dx = mouseDX;
		mouseDX = 0;
		return dx;
	}

	float GetDY()
	{
		float dy = mouseDY;
		mouseDY = 0;
		return dy;
	}

	void SetDScroll(float scroll_y)
	{
		deltaScroll += scroll_y;
	}

	float GetDScroll()
	{
		float dy = deltaScroll;
		deltaScroll = 0;
		return dy;
	}
};

struct KeyboardStates {
	bool keys[348];
	void Init()
	{
		for(unsigned int i = 0; i < 91; i++)
		{
			keys[i] = false;
		}
	}
	bool IsKeyDown(int keycode)
	{
		return keys[keycode];
	}

	void SetKeyStatus(int keycode, bool flag)
	{
		keys[keycode] = flag;
	}
};


class Camera : public EventListener, public EventDispatcher
{
public:
	//Camera(glm::vec3& pos);
    Camera(glm::vec3 pos, glm::vec3 rot);
	~Camera();

	/* Event stuff */
	void OnAttach();
	void OnEvent(const Event &e);

	void KeyEventRecieved(int keycode, bool flag);

	void Move(glm::vec3& dv, glm::vec3& dRot);
	void Update(float dt);
	
	glm::vec3& GetPosition();
	inline float GetZoom()  { return zoom;  }
	inline float GetRoll()  { return roll;  }
	inline float GetPitch() { return pitch; }
	inline float GetYaw()   { return yaw;   }

	void SetPivot(glm::vec3& pivot);
	void SetZoom(float z);
	void SetPitch(float p);
	void SetYaw(float y);
	
	void CalculatePosition(float dt);
	glm::mat4& GetProjectionMatrix();
	glm::mat4& ResetProjectionMatrix();
	glm::mat4& GetViewMatrix();

	const float DEFAULT_MOVE_SPEED = 6.5;
	const float DEFAULT_ROT_SPEED = 80;
	const double MIN_ZOOM = 22;
	const double MAX_ZOOM = -22;

private:
	void GenerateProjectionMatrix();

	MouseStates mouse;
	KeyboardStates keyboard;

	glm::vec3 position;
	glm::vec3 velocity;

	/* --- */
	glm::vec3 deltaPos;
	glm::vec3 deltaRot;
	/* --- */

	glm::vec3 pivot;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	float yaw;
	float pitch;
	float roll;
	float zoom;
	float zoomSpeed;
	float moveSpeed;
	float cam_aspect_ratio = 1920.0f/1080.0f;
	float angleAroundPivot;
	float distToPivot;

	const float FOV = 60.0f;				/* 60 -> ~80 is recommended */
	const float NEAR_PLANE = 0.1f;			/* min clipping dist. */
	const float FAR_PLANE = 10000.0f;		/* max clipping dist. */

};
}
#pragma once
#include "pch.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

namespace juno { 
class Camera : public EventListener
{
public:
	//Camera(glm::vec3& pos);
    Camera(glm::vec3 pos, float yaw, float pitch, float roll);
	~Camera() override;

	/* Event stuff */
	void onAttach();
	void onEvent(const KeyEvent &e);
	void onEvent(const MouseEvent &e);
	void onEvent(const Event &e);

	void move(glm::vec3& dv, glm::vec3& dRot);
	void update(glm::vec3& dPos, glm::vec3& dRot, float deltaZoom);
	
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
	const double MIN_ZOOM = 32;
	const double MAX_ZOOM = -40;

private:
	void generateProjectionMatrix();

	glm::vec3 position;
	glm::vec3 velocity;
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
	
	const float FOV = 60.0f;				/* 60 -> ~80 is recommended */
	const float NEAR_PLANE = 0.1f;			/* min clipping dist. */
	const float FAR_PLANE = 1000.0f;		/* max clipping dist. */

};
}
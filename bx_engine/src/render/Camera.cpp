#include "render/Camera.h"
#include "core/bxMath.h"
#include "core/EngineConfig.h"
using namespace bx;



Camera::Camera(glm::vec3 pos, float yaw, float pitch, float roll) : position(pos)
{

	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
	this->moveSpeed = 2.5f;
	this->mouseSpeed = 0.1f;
	this->zoom = 0.0f;
	generateProjectionMatrix();

}

Camera::~Camera()
{

}

void Camera::move(glm::vec3 pos, glm::vec3 dRot)
{
	 this->position = pos;
	 this->pitch += dRot.y;
	 this->yaw += dRot.z;
	 this->roll += dRot.x;
}



void Camera::update(glm::vec3 dPos, glm::vec3 dRot, float deltaZoom)
{
	this->position += dPos; 
	this->roll += dRot.x;
	this->pitch += dRot.y;
	this->yaw += dRot.z;
	this->zoom += deltaZoom;
}

glm::vec3& Camera::getPosition()
{
	return this->position;
}

float Camera::getZoom()
{
	return this->zoom;
}

float Camera::getRoll()
{
	return this->roll;
}

float Camera::getPitch()
{
	return this->pitch;
}

float Camera::getYaw()
{
	return this->yaw;
}

void Camera::setZoom(float z)
{
	this->zoom = z;
}

glm::mat4& Camera::getProjectionMatrix()
{
	return this->projectionMatrix;
}

void Camera::generateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(FOV+zoom), getAspectRatio(), NEAR_PLANE, FAR_PLANE);	
}

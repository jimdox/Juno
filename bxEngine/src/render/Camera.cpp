#include "render/Camera.h"
using namespace bbx;

Camera::Camera(glm::vec3 pos) : position(pos)
{
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
	this->moveSpeed = 2.5f;
	this->mouseSpeed = 0.1f;
	this->zoom = 45.0f;
	update();

}

Camera::Camera(glm::vec3 pos, glm::vec3 upDir, float yaw, float pitch) : position(pos), up(upDir)
{
	globalUp = up;
	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = 0.0f;
	this->moveSpeed = 2.5f;
	this->mouseSpeed = 0.1f;
	this->zoom = 45.0f;
	update();

}

Camera::~Camera()
{

}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::update()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, globalUp));
	up = glm::normalize(glm::cross(right, front));
}

float Camera::getZoom()
{
	return zoom;
}

float Camera::getRoll()
{
	return roll;
}

float Camera::getPitch()
{
	return pitch;
}

float Camera::getYaw()
{
	return yaw;
}
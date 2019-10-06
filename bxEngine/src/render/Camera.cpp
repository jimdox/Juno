#include "render/Camera.h"
using namespace bbx;

Camera::Camera(glm::vec3& pos, glm::vec3& axisUp, glm::vec3& rot)
{
	this->positition = pos;

	this->globalUp = axisUp;

	this->rotation = rot;

	this->zoom = 0.0f;
	this->front = glm::vec3(0.0f);
	this->up = glm::vec3(0.0f);
	this->mouseSensitivity = 1.0f;

}

Camera::~Camera()
{

}

glm::mat4& Camera::getViewMat()
{
	glm::mat4 test(1.0f); /* impl. */
	return test;
}

void Camera::update()
{

}




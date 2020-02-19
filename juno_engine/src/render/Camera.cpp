#include "render/Camera.h"
#include "core/EngineConfig.h"
#include "core/Log.h"
#include "core/MathUtils.h"

using namespace juno;



Camera::Camera(glm::vec3 pos, float yaw, float pitch, float roll) : position(pos)
{

	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
	this->moveSpeed = 2.5f;
	this->zoom = 0.0f;
	generateProjectionMatrix();
	velocity = glm::vec3(0,0,0);

}

Camera::~Camera()
{
		
}

void Camera::onAttach()
{

}

void Camera::onEvent(const Event &e)
{
	switch (e.getType())
	{
	case EventType::KEY_PRESS:
		
		break;
	case EventType::KEY_RELEASE:

		break;
	case EventType::MOUSE_BUTTON_PRESS:

		break;
	case EventType::MOUSE_BUTTON_RELEASE:

		break;
	case EventType::MOUSE_MOVE:

		break;
	case EventType::MOUSE_SCROLL:

		break;
	default:
		JN_WARN("Camera recieved an unknown event type.");
		break;
	}
}

void Camera::move(glm::vec3& pos, glm::vec3& dRot)
{

}




void Camera::update(glm::vec3& dPos, glm::vec3& dRot, float deltaZoom)
{



}


/* angle must be in radians */
void Camera::calculateCameraPos()
{
	
	
}

glm::mat4& Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::generateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(FOV+zoom), getAspectRatio(), NEAR_PLANE, FAR_PLANE);	
}

glm::mat4& Camera::resetProjectionMatrix()
{
	generateProjectionMatrix();
	return projectionMatrix;
}

void Camera::setPivot(glm::vec3& pivot)
{
	this->pivot = pivot;
}

glm::vec3& Camera::getPosition()
{
	return position;
}

void Camera::setZoom(float z)
{
	//JN_ERR("zoom {}", z);
	if(z < MAX_ZOOM)
	{
		this->zoom = MAX_ZOOM;
	} else if(z > MIN_ZOOM) {
		this->zoom = MIN_ZOOM;
	} else {
	this->zoom = z;
	}
}
#include "render/Camera.h"
#include "core/EngineConfig.h"
#include "core/Log.h"
#include "core/MathUtils.h"
//#include "core/InputStates.h"
//#include "render/Context.h"
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

void Camera::onEvent(const KeyEvent &e)
{
	if(e.getType() == EventType::KEY_PRESS)
	{
		switch (e.getKeyCode())
		{
		case GLFW_KEY_W:
			
			break;
		case GLFW_KEY_S:

			break;
		
		
		default:
			break;
		}
	} else /* then KeyReleaseEvent */
	{

	}
}

void Camera::onEvent(const MouseEvent &e)
{

}

void Camera::onEvent(const Event &e)
{
	switch (e.getType())
	{
	case EventType::KEY_PRESS:
		JN_WARN("Something went terribly wrong...");
		break;
	
	default:
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
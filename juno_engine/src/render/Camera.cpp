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
	lmb_pressed = false;

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
		keyPressRecieved(((const KeyPressEvent&) e).getKeyCode());
		break;
	case EventType::KEY_RELEASE:
		//((const KeyReleaseEvent&) e).getKeyCode()
		break;
	case EventType::MOUSE_BUTTON_PRESS:
		mouseBPressRecieved((const MousePressEvent&) e);
		break;
	case EventType::MOUSE_BUTTON_RELEASE:
		mouseBReleaseRecieved((const MouseReleaseEvent&)e);
		break;
	case EventType::MOUSE_MOVE:
		mouseMoveRecieved((const MouseMoveEvent&)e);
		break;
	case EventType::MOUSE_SCROLL:
		setZoom(this->zoom + ((const MouseScrollEvent&)e).getYOffset() * 5);
		break;
	default:
		JN_WARN("Camera recieved an unknown event type.");
		break;
	}
}

void Camera::keyPressRecieved(int key_code)
{
	JN_CRIT("key recieved");
	switch(key_code)
	{
	case GLFW_KEY_W:
		setZoom(this->zoom - 0.01);
		break;
	case GLFW_KEY_S:

		break;
	case GLFW_KEY_A:

		break;
	case GLFW_KEY_D:
		break;


	case GLFW_KEY_ESCAPE:
		this->notify(WindowCloseEvent());
		break;
	default:

		break;
	}
	
}

void Camera::mouseBPressRecieved(const MousePressEvent& e)
{
	auto a = e.getMouseCode();
	if(e.getMouseCode() == MouseCode::M_BUTTON_LEFT)
	{
		lmb_pressed = true;
	}
}

void Camera::mouseBReleaseRecieved(const MouseReleaseEvent& e)
{
	if(e.getMouseCode() == MouseCode::M_BUTTON_LEFT)
	{
		lmb_pressed = false;
	}
}

void Camera::mouseMoveRecieved(const MouseMoveEvent& e)
{


	if(lmb_pressed)
	{
		pitch -= (e.getY() - prev_mouse_y) * 0.14f;

		/* calculate angle around pivot */
		
		/* calculate horizontal dist. */
		float h_dist = ((position - pivot).length() - zoom) * cosf(pitch);
		float v_dist = ((position - pivot).length() - zoom) * sinf(pitch);
	}
	prev_mouse_y = e.getY();
	prev_mouse_x = e.getX();
}
void Camera::move(glm::vec3& pos, glm::vec3& dRot)
{

}




void Camera::update()
{

	resetProjectionMatrix();

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
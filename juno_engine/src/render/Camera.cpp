#include "render/Camera.h"
#include "core/EngineConfig.h"
#include "core/Log.h"
#include "core/MathUtils.h"
#include "events/RenderEvent.h"
using namespace juno;


Camera::Camera(glm::vec3 pos, glm::vec3 rot) : pivot(pos)
{
	this->roll = rot.x;
	this->pitch = rot.y;
	this->yaw = rot.z;
	this->moveSpeed = 2.5f;
	this->zoom = 0.0f;
	velocity = glm::vec3(0,0,0);
	angle_around_pivot = 180.0f;
	distance_to_pivot = 50.0f;
	calculatePosition();
	generateProjectionMatrix();
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
		keyEventRecieved(((const KeyPressEvent&) e).getKeyCode(), true);
		break;
	case EventType::KEY_RELEASE:
		keyEventRecieved(((const KeyReleaseEvent&)e).getKeyCode(), false);
		break;
	case EventType::MOUSE_BUTTON_PRESS:
		mouse.setButton(((const MousePressEvent&) e).getMouseCode(), true);
		break;
	case EventType::MOUSE_BUTTON_RELEASE:
		mouse.setButton(((const MouseReleaseEvent&)e).getMouseCode(), false);
		break;
	case EventType::MOUSE_MOVE:
		//const MouseMoveEvent& mouse_event = ((const MouseMoveEvent&)e);
		mouse.setPosition(((const MouseMoveEvent&)e).getX(), ((const MouseMoveEvent&)e).getY());
		break;
	case EventType::MOUSE_SCROLL:
		mouse.setDScroll(this->zoom - ((const MouseScrollEvent&)e).getYOffset() * 3);
		break;
	case EventType::WINDOW_RESIZE:
		this->cam_aspect_ratio = ((const WindowResizeEvent&)e).getScreenWidth() / ((const WindowResizeEvent&)e).getScreenHeight();
		break;
	default:
		JN_WARN("Camera recieved an event of unknown type.");
		break;
	}
}

/* camera only processes certain mouse keys, other key events are discarded for now. */
void Camera::keyEventRecieved(int key_code, bool flag)
{
	switch(key_code)
	{
	case GLFW_KEY_W:
	case GLFW_KEY_S:
	case GLFW_KEY_A:
	case GLFW_KEY_D:
	case GLFW_KEY_R:
	case GLFW_KEY_F:
			keyboard.setKeyStatus(key_code, flag);
			break;
	case GLFW_KEY_X:
		keyboard.setKeyStatus(GLFW_KEY_X, flag);
		notify(RenderWireframeEvent(flag));
		break;
	case GLFW_KEY_ESCAPE:
		this->notify(WindowCloseEvent());
		break;
	default:
		break;
	}
	
}

void Camera::move(glm::vec3& pos, glm::vec3& dRot)
{

}



void Camera::update()
{
	if(keyboard.isKeyDown(GLFW_KEY_W))
	{
		pivot.z -= 0.2f;  
	} else if(keyboard.isKeyDown(GLFW_KEY_S))
	{
		pivot.z += 0.2f;
	}

	if(keyboard.isKeyDown(GLFW_KEY_R))
	{
		pivot.y += 0.2f;
	} else if(keyboard.isKeyDown(GLFW_KEY_F)) {
		pivot.y -= 0.2f;
	}

	if(keyboard.isKeyDown(GLFW_KEY_D))
	{
		pivot.x += 0.2f;
	} else if(keyboard.isKeyDown(GLFW_KEY_A)) {
		pivot.x -= 0.2f;
	}

	calculatePosition();
	resetProjectionMatrix();
}


/* 3rd person camera */
void Camera::calculatePosition()
{
	distance_to_pivot += mouse.getDScroll()*1.5f;

	if(mouse.isButtonDown(MouseCode::M_BUTTON_MID))
	{
		float delta_pitch = mouse.getDY() * 0.22f;
		pitch -= delta_pitch;
		float angle_delta = mouse.getDX() * 0.2f;
		angle_around_pivot += angle_delta;
	}

	float h_distance = distance_to_pivot * cosf(toRadians(pitch));
	float v_distance = distance_to_pivot * sinf(toRadians(pitch)); 

	float theta = angle_around_pivot;

	float x_offset = h_distance * sinf(toRadians(theta));
	float z_offset = h_distance * cosf(toRadians(theta));
	
	yaw = 180.0f - angle_around_pivot;
	position.x = pivot.x - x_offset; 
	position.z = pivot.z - z_offset;
	position.y = pivot.y + v_distance;
}

glm::mat4& Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::generateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(FOV+zoom), cam_aspect_ratio, NEAR_PLANE, FAR_PLANE);	
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
	if(z < MAX_ZOOM)
	{
		this->zoom = MAX_ZOOM;
	} else if(z > MIN_ZOOM) {
		this->zoom = MIN_ZOOM;
	} else {
	this->zoom = z;
	}
}
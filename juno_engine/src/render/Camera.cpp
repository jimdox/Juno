#include "render/Camera.h"
#include "core/EngineConfig.h"
#include "core/Log.h"
#include "utils/MathUtils.h"
#include "events/RenderEvent.h"
using namespace juno;

Camera::Camera(glm::vec3 pos, glm::vec3 rot) : pivot(pos)
{
	this->roll = rot.x;
	this->pitch = rot.y;
	this->yaw = rot.z;
	this->moveSpeed = 2.5f;
	this->zoom = 0.0f;
	position = glm::vec3(0,0,0);
	velocity = glm::vec3(0,0,0);
	angle_around_pivot = 180.0f;
	keyboard.init();
	//mouse.setDScroll(25);

	distance_to_pivot += 30;
	distance_to_pivot = max(distance_to_pivot, 0.001f);
	calculatePosition(0);
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
	// switch(key_code)
	// {
	// case GLFW_KEY_W:
	// case GLFW_KEY_S:
	// case GLFW_KEY_A:
	// case GLFW_KEY_D:
	// case GLFW_KEY_R:
	// case GLFW_KEY_F:
	// 		break;
	// default:
	// 	break;
	// }
	keyboard.setKeyStatus(key_code, flag);

}

void Camera::move(glm::vec3& pos, glm::vec3& dRot)
{

}

void Camera::update(float dt)
{

	calculatePosition(dt);
}

/* 3rd person camera */
void Camera::calculatePosition(float dt)
{
	delta_pos = glm::vec3(0,0,0);
	
	if(keyboard.isKeyDown(GLFW_KEY_UP))
	{
		delta_pos.z = cosf(toRadians(angle_around_pivot));  
		delta_pos.x = sinf(toRadians(angle_around_pivot));
	} else if(keyboard.isKeyDown(GLFW_KEY_DOWN))
	{
		delta_pos.z = -cosf(toRadians(angle_around_pivot));
		delta_pos.x = -sinf(toRadians(angle_around_pivot));
	}

	if(keyboard.isKeyDown(GLFW_KEY_RIGHT))
	{
		delta_pos.x -= cosf(toRadians(angle_around_pivot));
		delta_pos.z += sinf(toRadians(angle_around_pivot));
	} else if(keyboard.isKeyDown(GLFW_KEY_LEFT)) {
		delta_pos.x += cosf(toRadians(angle_around_pivot));
		delta_pos.z -= sinf(toRadians(angle_around_pivot));
	}

	// delta_pos *= CAM_MOVE_SPEED;
	pivot += delta_pos;

	float deltaScroll = mouse.getDScroll();
	if(deltaScroll == 0 && abs(zoomSpeed) > 0)
	{
		zoomSpeed -= zoomSpeed * 0.082f;
	} else {
		zoomSpeed += deltaScroll * 24.0f;
	}
	distance_to_pivot += zoomSpeed * dt;
	distance_to_pivot = max(distance_to_pivot, 0.001f);

	if(mouse.isButtonDown(MouseCode::M_BUTTON_RIGHT) || mouse.isButtonDown(MouseCode::M_BUTTON_MID))
	{
		if(keyboard.isKeyDown(GLFW_KEY_LEFT_SHIFT) || keyboard.isKeyDown(GLFW_KEY_RIGHT_SHIFT))
		{
			float dx = mouse.getDX() * max(distance_to_pivot, 2.0f)/1000.0f;
			float dy = mouse.getDY() * max(distance_to_pivot, 2.0f)/1000.0f;

			pivot.x -= dx * cosf(toRadians(yaw)) - dy * sinf(toRadians(pitch)) * sinf(toRadians(yaw));
			pivot.y += dy * cosf(toRadians(pitch));
			pivot.z -= dx * sinf(toRadians(yaw)) + dy * sinf(toRadians(pitch)) * cosf(toRadians(yaw));

		} else {
			pitch -= mouse.getDY() * 0.255f;
			angle_around_pivot += mouse.getDX() * 0.255f;
		}
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
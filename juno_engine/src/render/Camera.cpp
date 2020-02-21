#include "render/Camera.h"
#include "core/EngineConfig.h"
#include "core/Log.h"
#include "core/MathUtils.h"
#include "events/RenderEvent.h"
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
		keyEventRecieved(((const KeyPressEvent&) e).getKeyCode(), true);
		break;
	case EventType::KEY_RELEASE:
		keyEventRecieved(((const KeyReleaseEvent&)e).getKeyCode(), false);
		break;
	case EventType::MOUSE_BUTTON_PRESS:
		io_states.setMouseButtonStatus(((const MousePressEvent&) e).getMouseCode(), true);
		break;
	case EventType::MOUSE_BUTTON_RELEASE:
		io_states.setMouseButtonStatus(((const MouseReleaseEvent&)e).getMouseCode(), false);
		break;
	case EventType::MOUSE_MOVE:
		//const MouseMoveEvent& mouse_event = ((const MouseMoveEvent&)e);
		io_states.mouseMoved(((const MouseMoveEvent&)e).getX(), ((const MouseMoveEvent&)e).getY());
		break;
	case EventType::MOUSE_SCROLL:
		setZoom(this->zoom - ((const MouseScrollEvent&)e).getYOffset() * 5);
		break;
	case EventType::WINDOW_RESIZE:
		this->cam_aspect_ratio = ((const WindowResizeEvent&)e).getScreenWidth() / ((const WindowResizeEvent&)e).getScreenHeight();
		break;
	default:
		JN_WARN("Camera recieved an event of unknown type.");
		break;
	}
}

void Camera::keyEventRecieved(int key_code, bool flag)
{
	JN_CRIT("key recieved");
	switch(key_code)
	{
	case GLFW_KEY_W:
		io_states.setKeyStatus(GLFW_KEY_W, flag);
		break;
	case GLFW_KEY_S:
		io_states.setKeyStatus(GLFW_KEY_S, flag);
		break;
	case GLFW_KEY_A:
		io_states.setKeyStatus(GLFW_KEY_A, flag);
		break;
	case GLFW_KEY_D:
		io_states.setKeyStatus(GLFW_KEY_D, flag);
		break;
	case GLFW_KEY_X:
		io_states.setKeyStatus(GLFW_KEY_X, flag);
		notify(RenderWireframeEvent(flag));
		break;
	case GLFW_KEY_ESCAPE:
		this->notify(WindowCloseEvent());
		break;
	default:
		break;
	}
	
}




// void Camera::mouseMoveRecieved(const MouseMoveEvent& e)
// {


// 	if(io_states.isMButtonDown(MouseCode::M_BUTTON_LEFT))
// 	{
// 		pitch -= io_states.getMouseDY() * 0.14f;
// 		yaw += io_states.getMouseDX() * 0.1f;
// 		/* calculate angle around pivot */
		
// 		/* calculate horizontal dist. */
// 		float h_dist = ((position - pivot).length() - zoom) * cosf(pitch);
// 		float v_dist = ((position - pivot).length() - zoom) * sinf(pitch);
// 	}

// }

void Camera::move(glm::vec3& pos, glm::vec3& dRot)
{

}




void Camera::update()
{
	if(io_states.isMButtonDown(MouseCode::M_BUTTON_LEFT))
	{
		pitch -= io_states.getMouseDY() * 0.1f;
		yaw -= io_states.getMouseDX() * 0.1f;
	}

	if(io_states.isKeyDown(GLFW_KEY_W))
	{
		this->position.z -= 0.001f;
	}



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
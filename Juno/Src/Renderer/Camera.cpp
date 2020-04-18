#include "jnpch.h"
#include "Renderer/Camera.h"
#include "Core/Config.h"
#include "Core/Log.h"
#include "Utils/Math.h"
#include "Events/RenderEvents.h"
using namespace Juno;

Camera::Camera(glm::vec3 pos, glm::vec3 rot) : pivot(pos)
{
	this->roll = rot.x;
	this->pitch = rot.y;
	this->yaw = rot.z;
	this->moveSpeed = 2.5f;
	this->zoom = 0.0f;
	position = glm::vec3(0,0,0);
	velocity = glm::vec3(0,0,0);
	angleAroundPivot = 180.0f;
	keyboard.Init();
	//mouse.setDScroll(25);

	distToPivot += 30;
	distToPivot = Max(distToPivot, 0.001f);
	CalculatePosition(0);
	GenerateProjectionMatrix();
}

Camera::~Camera()
{
		
}

void Camera::OnAttach()
{

}

void Camera::OnEvent(const Event &e)
{	
	switch (e.GetType())
	{
	case EventType::KEY_PRESS:
		KeyEventRecieved(((const KeyPressEvent&) e).GetKeyCode(), true);
		break;
	case EventType::KEY_RELEASE:
		KeyEventRecieved(((const KeyReleaseEvent&)e).GetKeyCode(), false);
		break;
	case EventType::MOUSE_BUTTON_PRESS:
		mouse.SetButton(((const MousePressEvent&) e).GetMouseCode(), true);
		break;
	case EventType::MOUSE_BUTTON_RELEASE:
		mouse.SetButton(((const MouseReleaseEvent&)e).GetMouseCode(), false);
		break;
	case EventType::MOUSE_MOVE:
		//const MouseMoveEvent& mouse_event = ((const MouseMoveEvent&)e);
		mouse.SetPosition(((const MouseMoveEvent&)e).GetX(), ((const MouseMoveEvent&)e).GetY());
		break;
	case EventType::MOUSE_SCROLL:
		mouse.SetDScroll(this->zoom - ((const MouseScrollEvent&)e).GetYOffset() * 3);
		break;
	case EventType::WINDOW_RESIZE:
		this->cam_aspect_ratio = ((const WindowResizeEvent&)e).GetScreenWidth() / ((const WindowResizeEvent&)e).GetScreenHeight();
		break;
	default:
		JN_WARN("Camera recieved an event of unknown type.");
		break;
	}
}

/* camera only processes certain mouse keys, other key events are discarded for now. */
void Camera::KeyEventRecieved(int key_code, bool flag)
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
	keyboard.SetKeyStatus(key_code, flag);

}

void Camera::Move(glm::vec3& pos, glm::vec3& dRot)
{

}

void Camera::Update(float dt)
{

	CalculatePosition(dt);
}

/* 3rd person camera */
void Camera::CalculatePosition(float dt)
{
	deltaPos = glm::vec3(0,0,0);
	
	if(keyboard.IsKeyDown(GLFW_KEY_UP))
	{
		deltaPos.z = cosf(ToRadians(angleAroundPivot));  
		deltaPos.x = sinf(ToRadians(angleAroundPivot));
	} else if(keyboard.IsKeyDown(GLFW_KEY_DOWN))
	{
		deltaPos.z = -cosf(ToRadians(angleAroundPivot));
		deltaPos.x = -sinf(ToRadians(angleAroundPivot));
	}

	if(keyboard.IsKeyDown(GLFW_KEY_RIGHT))
	{
		deltaPos.x -= cosf(ToRadians(angleAroundPivot));
		deltaPos.z += sinf(ToRadians(angleAroundPivot));
	} else if(keyboard.IsKeyDown(GLFW_KEY_LEFT)) {
		deltaPos.x += cosf(ToRadians(angleAroundPivot));
		deltaPos.z -= sinf(ToRadians(angleAroundPivot));
	}
	pivot += deltaPos;

	float deltaScroll = mouse.GetDScroll();
	if(deltaScroll == 0 && zoomSpeed != 0)
	{
		if(abs(zoomSpeed) < 0.03f)
		{
			zoomSpeed = 0;
		}else {
			zoomSpeed -= zoomSpeed * 0.16f;
		}
	} else {
		zoomSpeed += deltaScroll * 20.0f;
	}

	distToPivot += zoomSpeed * dt;
	distToPivot = Max(distToPivot, 0.001f);

	if(mouse.IsButtonDown(MouseCode::M_BUTTON_RIGHT) || mouse.IsButtonDown(MouseCode::M_BUTTON_MID))
	{
		if(keyboard.IsKeyDown(GLFW_KEY_LEFT_SHIFT) || keyboard.IsKeyDown(GLFW_KEY_RIGHT_SHIFT))
		{
			float dx = mouse.GetDX() * Max(distToPivot, 2.0f)/1000.0f;
			float dy = mouse.GetDY() * Max(distToPivot, 2.0f)/1000.0f;

			pivot.x -= dx * cosf(ToRadians(yaw)) - dy * sinf(ToRadians(pitch)) * sinf(ToRadians(yaw));
			pivot.y += dy * cosf(ToRadians(pitch));
			pivot.z -= dx * sinf(ToRadians(yaw)) + dy * sinf(ToRadians(pitch)) * cosf(ToRadians(yaw));

		} else {
			pitch -= mouse.GetDY() * 0.255f;
			angleAroundPivot += mouse.GetDX() * 0.255f;
		}
	}

	float h_distance = distToPivot * cosf(ToRadians(pitch));
	float v_distance = distToPivot * sinf(ToRadians(pitch)); 

	float theta = angleAroundPivot;
	float x_offset = h_distance * sinf(ToRadians(theta));
	float z_offset = h_distance * cosf(ToRadians(theta));
	
	yaw = 180.0f - angleAroundPivot;
	position.x = pivot.x - x_offset; 
	position.z = pivot.z - z_offset;
	position.y = pivot.y + v_distance;
}

glm::mat4& Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::GenerateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(FOV+zoom), cam_aspect_ratio, NEAR_PLANE, FAR_PLANE);	
}

glm::mat4& Camera::ResetProjectionMatrix()
{
	GenerateProjectionMatrix();
	return projectionMatrix;
}

void Camera::SetPivot(glm::vec3& pivot)
{
	this->pivot = pivot;
}

glm::vec3& Camera::GetPosition()
{
	return position;
}

void Camera::SetZoom(float z)
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
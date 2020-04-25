#include "jnpch.h"
#include "Renderer/RenderObjects/ThirdPersonCamera.h"
#include "Events/KeyEvents.h"
#include <GLFW/glfw3.h>

using namespace Juno;


ThirdPersonCamera::ThirdPersonCamera(const Vec3& position, const Vec3& rotation) : Camera(position, rotation)
{
    angleAroundPivot = M_PI;
    distToPivot = 30;
	zoom = 0;
	Update(0);
	for(int i = 0; i < 348; i++)
	{
		keyboard.SetKeyStatus(i, false);
	}
	GenerateProjectionMatrix();

}

ThirdPersonCamera::~ThirdPersonCamera()
{

}

void ThirdPersonCamera::OnAttach()
{

}

void ThirdPersonCamera::OnEvent(const Event &e)
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
		mouse.SetDScroll(((const MouseScrollEvent&)e).GetYOffset() * -3);
		break;
	case EventType::WINDOW_RESIZE:
		this->camAspectRatio = ((const WindowResizeEvent&)e).GetScreenWidth() / ((const WindowResizeEvent&)e).GetScreenHeight();
		break;
	default:
		JN_WARN("Camera recieved an event of unknown type.");
		break;
	}
}

void ThirdPersonCamera::KeyEventRecieved(int keyCode, bool isPressed)
{
    keyboard.SetKeyStatus(keyCode, isPressed);
}

void ThirdPersonCamera::Update(float dt)
{

	distToPivot += mouse.GetDScroll() * 2 * distToPivot *  dt;
	distToPivot = Math::Max(distToPivot, NEAR_PLANE);

	if(mouse.IsButtonDown(MouseCode::M_BUTTON_RIGHT) || mouse.IsButtonDown(MouseCode::M_BUTTON_MID))
	{
		if(keyboard.IsKeyDown(GLFW_KEY_LEFT_SHIFT))
		{
			float dx = mouse.GetDX() * Math::Max(distToPivot, 2.0f)/500.0f;
			float dy = mouse.GetDY() * Math::Max(distToPivot, 2.0f)/500.0f;

			pivot.x -= dx * cosf(yaw) - dy * sinf(pitch) * sinf(yaw);
			pivot.y += dy * cosf(pitch);
			pivot.z -= dx * sinf(yaw) + dy * sinf(pitch) * cosf(yaw);

		} else {
			pitch -= mouse.GetDY()/200;
			angleAroundPivot += mouse.GetDX()/200;
		}
	}

	float h_distance = distToPivot * cosf(pitch);
	float v_distance = distToPivot * sinf(pitch); 

	float theta = angleAroundPivot;
	float x_offset = h_distance * sinf(theta);
	float z_offset = h_distance * cosf(theta);
	
	yaw = M_PI - angleAroundPivot;
	position.x = pivot.x - x_offset; 
	position.z = pivot.z - z_offset;
	position.y = pivot.y + v_distance;
}
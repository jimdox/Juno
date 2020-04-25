#pragma once
#include "Utils/Math.h"
#include "Renderer/RenderObjects/Camera.h"

namespace Juno
{
class ThirdPersonCamera : public Camera
{
public:
    ThirdPersonCamera(const Vec3& position, const Vec3& rotation);
    ~ThirdPersonCamera();

    void OnAttach() override;
    void OnEvent(const Event& e) override;
    void Update(float dt) override;

    void Move();

protected:
	float zoomSpeed;
	float moveSpeed;
    float angleAroundPivot;
	float distToPivot;
	const float DEFAULT_MOVE_SPEED = 6.5;
	const float DEFAULT_ROT_SPEED = 80;
    Vec3 deltaPos;

private:
    void KeyEventRecieved(int keyCode, bool isPressed);

};
}
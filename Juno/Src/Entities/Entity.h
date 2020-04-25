#pragma once
#include "Renderer/RenderObjects/Mesh.h"

namespace Juno {
class Entity
{
public:
	Entity(SPtr<Mesh> mesh, const std::string& name);
	Entity(SPtr<Mesh> mesh, const Vec3& pos, const Vec3& rot, float scale, const std::string& name);	
	~Entity();

	SPtr<Mesh>& GetMesh();
	Vec3& GetPosition();
	Vec3& GetRotation();
	Vec3& GetVelocity();
	Vec3& GetRotVelocity();
	float GetScale();
	std::string& GetName();

	void SetPosition(const Vec3& pos);
	void SetRotation(const Vec3& rot);
	void SetVelocity(const Vec3& vel);
	void SetRotVelocity(const Vec3& rotVel);
	void SetScale(float scale);

	void AddPosition(const Vec3& deltaPos);
	void Update();

private:
	std::string name;
	SPtr<Mesh> mesh;
	Vec3 position;
	Vec3 rotation;
	Vec3 velocity;
	Vec3 rotVelocity;
	
	float scale;
	bool visible;
};
}


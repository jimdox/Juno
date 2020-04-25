#include "jnpch.h"
#include "Entities/Entity.h"

// /#include "core/AssetManager.h"
using namespace Juno;


Entity::Entity(SPtr<Mesh> mesh, const std::string& name) : mesh(mesh), name(name)
{
	/* initializes rotation, position to 0 vector */
	this->scale = 0.0f;
	this->visible = true;
}

Entity::Entity(SPtr<Mesh> mesh, const glm::vec3& pos, const glm::vec3& rot, float scale, const std::string& name) : mesh(mesh), name(name), position(pos), rotation(rot)
{
	this->scale = scale;
	this->visible = true;
}

Entity::~Entity()
{
	
}

SPtr<Mesh>& Entity::GetMesh()
{
	return this->mesh;
}

glm::vec3& Entity::GetPosition()
{
	return this->position;
}

glm::vec3& Entity::GetRotation()
{
	return this->rotation;
}

glm::vec3& Entity::GetVelocity()
{
	return velocity;
}

glm::vec3& Entity::GetRotVelocity()
{
	return rotVelocity;
}

float Entity::GetScale()
{
	return this->scale;
}

std::string& Entity::GetName()
{
	return this->name;
}

void Entity::SetPosition(const glm::vec3& pos)
{
	this->position = pos;
}

void Entity::SetRotation(const glm::vec3& rot)
{
	this->rotation = rot;
}

void Entity::SetVelocity(const glm::vec3& vel)
{
	this->velocity = vel;
}

void Entity::SetRotVelocity(const glm::vec3& rotVel)
{
	this->rotVelocity = rotVel;
}

void Entity::SetScale(float s)
{
	this->scale = s;
}

void Entity::AddPosition(const glm::vec3& deltaPos)
{
	position += deltaPos;
}

void Entity::Update()
{


}

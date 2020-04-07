#include "entity/Entity.h"
#include "core/AssetLoader.h"
using namespace juno;


Entity::Entity(Mesh& mesh, const std::string& name) : mesh(mesh), name(name)
{
	/* initializes rotation, position to 0 vector */
	this->scale = 0.0f;
	this->visible = true;
}

Entity::Entity(Mesh& mesh, const glm::vec3& pos, const glm::vec3& rot, float scale, const std::string& name) : mesh(mesh), name(name), position(pos), rotation(rot)
{
	this->scale = scale;
	this->visible = true;
}

Entity::~Entity()
{
	
}

Mesh& Entity::getMesh()
{
	return this->mesh;
}

glm::vec3& Entity::getPosition()
{
	return this->position;
}

glm::vec3& Entity::getRotation()
{
	return this->rotation;
}

float Entity::getScale()
{
	return this->scale;
}

std::string& Entity::getName()
{
	return this->name;
}

void Entity::setPosition(const glm::vec3& pos)
{
	this->position = pos;
}

void Entity::setRotation(const glm::vec3& rot)
{
	this->rotation = rot;
}

void Entity::setScale(float s)
{
	this->scale = s;
}

void Entity::addPosition(const glm::vec3& delta_pos)
{
	position += delta_pos;
}

void Entity::update()
{


}

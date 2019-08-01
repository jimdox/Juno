#include "entity/Entity.h"

using namespace bbx;

Entity::Entity(Mesh& m, glm::mat4& transform) : mesh(m), transformMatrix(transform)
{


}

Entity::~Entity()
{

}

Mesh& Entity::getMesh()
{
	return this->mesh;
}

glm::mat4& Entity::getPosition()
{
	return this->transformMatrix;
}
#include "entity/Entity.h"

using namespace bbx;

Entity::Entity(Mesh& m) : mesh(m)
{


}

Entity::~Entity()
{

}

Mesh& Entity::getMesh()
{
	return this->mesh;
}


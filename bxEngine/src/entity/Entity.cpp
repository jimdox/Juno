#include "entity/Entity.h"
#include "core/bxAssetLoader.h"
using namespace bbx;

Entity::Entity(std::string& filepath, std::vector<Mesh>& meshes) :  assetFilePath(filepath), meshList(meshes)
{

}

Entity::Entity(std::string& filepath) : assetFilePath(filepath)
{
	loadModelData(filepath);

}
Entity::~Entity()
{

}

std::vector<Mesh>& Entity::getMeshList()
{
	return meshList;
}

void Entity::loadModelData(std::string& filepath)
{
	//bxImport::loadModel(filepath, &meshList)
}


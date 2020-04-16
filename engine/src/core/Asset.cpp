#include "core/Asset.h"
#include "core/AssetManager.h"
using namespace juno;

Asset::Asset() 
{
}

Asset::~Asset() {}

unsigned int Asset::getAssetID()
{
    return assetID;
}

void Asset::setAssetID(unsigned int assetID)
{
    this->assetID = assetID;
}
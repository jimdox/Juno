#pragma once
#include "pch.h"
namespace juno
{
class Asset
{
public:
    Asset();
    ~Asset();
    unsigned int getAssetID();

private:
    void setAssetID(unsigned int assetID);
    unsigned int assetID;
    friend class AssetManager;
};
}
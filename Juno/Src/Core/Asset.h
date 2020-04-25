#pragma once
namespace Juno
{
class Asset
{
public:
    Asset();
    ~Asset();
    unsigned int GetAssetID();

private:
    void SetAssetID(unsigned int assetID);
    unsigned int assetID;
    friend class AssetManager;
};
}
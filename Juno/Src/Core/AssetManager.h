#pragma once
#include "jnpch.h"
#include "Utils/Memory.h"
#include "Renderer/RenderObjects/Textures/CubeMap.h"
#include "Renderer/Data/Shader.h"
//#include "Renderer/RenderObjects/Shaders/ComputeShader.h"
#include "Entities/Entity.h"

#include "Renderer/Window.h"
#include "Renderer/Data/VertexArray.h"
#include "Renderer/Data/Texture.h"
/* using singleton schenanagans to control memory management
   in one instance across the program */
namespace Juno
{
class AssetManager
{
friend class Dock;
public:
    static AssetManager& Get()
    {   
        static AssetManager s_instance;
        return s_instance;
    }
    AssetManager(const AssetManager& assetManager) = delete;

    uint32_t FindAssetID(std::map<const std::string, uint32_t>& fileMap, const std::string& filepath);
    SPtr<Shader> LoadShader(std::array<ShaderComponentType, 3>& components, const std::string& filepath);

    //SPtr<ComputeShader> LoadComputeShader(const std::string& fp);
    
    std::array<uint32_t,4> LoadShaderFile(const std::string& filepath);
    std::string ReadShaderComponentFile(const std::string& filepath, ShaderComponentType shaderType);
    SPtr<Shader> GetDefaultShader();

    SPtr<Texture> LoadTexture(const std::string& filepath, TextureType tx_type);
    void LoadTextureFile(SPtr<Texture>& texture, const std::string& filepath, TextureType tx_type);

    //SPtr<CubeMap> LoadCubeMap(const std::string& filepath, TextureType tx_type);
    //uint32_t LoadCubeMapFiles(const std::array<std::string, 6>& filepaths, TextureType tx_type);
    
    SPtr<Mesh> LoadMesh(const std::string& filepath);

    std::tuple<VertexArray*, uint32_t, uint32_t> LoadOBJFile(const std::string& filepath);
    std::pair<VertexArray*, uint32_t> LoadToVAO(std::vector<float>& data, uint32_t dim);
    std::pair<VertexArray*, uint32_t> LoadToVAO(std::vector<float>& pos, std::vector<float>& textCoords, 
                                                     std::vector<float>& normals, std::vector<uint32_t>& indices);


private:
    AssetManager();
    uint32_t GenAssetID();

    uint32_t GenerateVAO();
    void StoreDataInAttribList(uint32_t attribNum, uint32_t components, std::vector<float> data);

    void ProcessFace(int faceVertices[3][3], std::vector<uint32_t>& indices, std::vector<glm::vec2>& textures, 
                    std::vector<glm::vec3> &normals, std::vector<float> &texturesData, std::vector<float>& normalsData);
    
    std::map<uint32_t, SPtr<Texture>>* GetTextureMap(); 
    std::map<uint32_t, SPtr<Shader>>* GetShaderMap();

    uint32_t currentIDPtr;

    /* todo: will switch to Juno::HashMap to improve access times on maps, but will need to research 
            good hashing function for filepaths, which will be difficult with the presence of 
            '/' or '\' chars possibly affecting collision probabilities. 
    */

    std::map<uint32_t, SPtr<Shader>> shaderRefs;
    std::map<uint32_t, SPtr<Texture>> textureRefs;
    std::map<uint32_t, SPtr<CubeMap>> cubeMapRefs;
    std::map<uint32_t, SPtr<Mesh>> meshRefs;

    std::map<const std::string, uint32_t> shaderFilepaths;
    std::map<const std::string, uint32_t> textureFilepaths;
    std::map<const std::string, uint32_t> cubeMapFilepaths;
    std::map<const std::string, uint32_t> meshFilepaths;
};
}
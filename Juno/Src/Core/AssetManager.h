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

    unsigned int FindAssetID(std::map<const std::string, unsigned int>& fileMap, const std::string& filepath);
    SPtr<Shader> LoadShader(const std::string& filepath);

    //SPtr<ComputeShader> LoadComputeShader(const std::string& fp);
    
    std::array<GLint,4> LoadShaderFile(const std::string& filepath);
    std::string ReadShaderComponentFile(const std::string& filepath, ShaderComponentType shaderType);
    SPtr<Shader> GetDefaultShader();

    SPtr<Texture> LoadTexture(const std::string& filepath, TextureType tx_type);
    void LoadTextureFile(SPtr<Texture>& texture, const std::string& filepath, TextureType tx_type);

    //SPtr<CubeMap> LoadCubeMap(const std::string& filepath, TextureType tx_type);
    //unsigned int LoadCubeMapFiles(const std::array<std::string, 6>& filepaths, TextureType tx_type);
    
    SPtr<Mesh> LoadMesh(const std::string& filepath);

    std::tuple<VertexArray*, unsigned int, unsigned int> LoadOBJFile(const std::string& filepath);
    std::pair<VertexArray*, unsigned int> LoadToVAO(std::vector<float>& data, unsigned int dim);
    std::pair<VertexArray*, unsigned int> LoadToVAO(std::vector<float>& pos, std::vector<float>& textCoords, 
                                                     std::vector<float>& normals, std::vector<unsigned int>& indices);


private:
    AssetManager(){}
    unsigned int GenAssetID();

    unsigned int GenerateVAO();
    void StoreDataInAttribList(unsigned int attribNum, unsigned int components, std::vector<float> data);

    void ProcessFace(int faceVertices[3][3], std::vector<unsigned int>& indices, std::vector<glm::vec2>& textures, 
                    std::vector<glm::vec3> &normals, std::vector<float> &texturesData, std::vector<float>& normalsData);
    
    std::map<unsigned int, SPtr<Texture>>* GetTextureMap(); 
    std::map<unsigned int, SPtr<Shader>>* GetShaderMap();

    unsigned int currentIDPtr;
    std::map<unsigned int, SPtr<Shader>> shaderRefs;
    std::map<unsigned int, SPtr<Texture>> textureRefs;
    std::map<unsigned int, SPtr<CubeMap>> cubeMapRefs;
    std::map<unsigned int, SPtr<Mesh>> meshRefs;

    std::map<const std::string, unsigned int> shaderFilepaths;
    std::map<const std::string, unsigned int> textureFilepaths;
    std::map<const std::string, unsigned int> cubeMapFilepaths;
    std::map<const std::string, unsigned int> meshFilepaths;
};
}
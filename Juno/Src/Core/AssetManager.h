#pragma once
#include "jnpch.h"
#include <stb_image.h>
#include <GL/glew.h>
#include "Renderer/Textures/CubeMap.h"
#include "Entities/Entity.h"
#include "Renderer/Shaders/Shader.h"
#include "Renderer/Shaders/ComputeShader.h"
#include "Gui/Dock.h"

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
    Shader& LoadShader(const std::string& filepath);

    ComputeShader& LoadComputeShader(const std::string& fp);
    
    std::array<GLint,4> LoadShaderFile(const std::string& filepath);
    GLint LoadShaderComponentFile(const std::string& filepath, GLenum shaderType);
    Shader& GetDefaultShader();

    //bool textureIsLoaded(const std::string& filepath);
    Texture& LoadTexture(const std::string& filepath, TextureType tx_type);
    unsigned int LoadTextureFile(const std::string& filepath, GLenum format, TextureType tx_type);

    CubeMap& LoadCubeMap(const std::string& filepath, TextureType tx_type);
    unsigned int LoadCubeMapFiles(const std::array<std::string, 6>& filepaths, TextureType tx_type);
    
    Mesh& LoadMesh(const std::string& filepath);

    std::tuple<unsigned int, unsigned int, unsigned int> LoadOBJFile(const std::string& filepath);
    std::pair<unsigned int, unsigned int> LoadToVAO(std::vector<float>& data, unsigned int dim);
    std::pair<unsigned int, unsigned int> LoadToVAO(std::vector<float>& pos, std::vector<float>& textCoords, 
                                                     std::vector<float>& normals, std::vector<unsigned int>& indices);


private:
    AssetManager(){}
    unsigned int GenAssetID();

    unsigned int GenerateVAO();
    void StoreDataInAttribList(unsigned int attribNum, unsigned int components, std::vector<float> data);

    void ProcessFace(int faceVertices[3][3], std::vector<unsigned int>& indices, std::vector<glm::vec2>& textures, 
                    std::vector<glm::vec3> &normals, std::vector<float> &texturesData, std::vector<float>& normalsData);
    
    std::map<unsigned int, std::unique_ptr<Texture>>* GetTextureMap(); 
    std::map<unsigned int, std::unique_ptr<Shader>>* GetShaderMap();

    unsigned int currentIDPtr;
    std::map<unsigned int, std::unique_ptr<Shader>> shaderRefs;
    std::map<unsigned int, std::unique_ptr<Texture>> textureRefs;
    std::map<unsigned int, std::unique_ptr<CubeMap>> cubeMapRefs;
    std::map<unsigned int, std::unique_ptr<Mesh>> meshRefs;

    std::map<const std::string, unsigned int> shaderFilepaths;
    std::map<const std::string, unsigned int> textureFilepaths;
    std::map<const std::string, unsigned int> cubeMapFilepaths;
    std::map<const std::string, unsigned int> meshFilepaths;
};
}
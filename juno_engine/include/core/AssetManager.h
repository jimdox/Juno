#pragma once
#include "pch.h"
#include <stb_image.h>
#include <GL/glew.h>
#include "render/textures/CubeMap.h"
#include "entity/Entity.h"
#include "render/shaders/Shader.h"
#include "gui/Dock.h"

/* using singleton schenanagans to control memory management
   in one instance across the program */
namespace juno
{
class AssetManager
{
friend class Dock;
public:
    static AssetManager& get()
    {   
        static AssetManager s_instance;
        return s_instance;
    }
    AssetManager(const AssetManager& assetManager) = delete;

    unsigned int findAssetID(std::map<const std::string, unsigned int>& fileMap, const std::string& filepath);
    Shader& loadShader(const std::string& filepath);
    
    GLint * loadShaderFile(const std::string& filepath);
    GLint loadShaderComponentFile(const std::string& filepath, GLenum shaderType);
    Shader& getDefaultShader();

    //bool textureIsLoaded(const std::string& filepath);
    Texture& loadTexture(const std::string& filepath, TextureType tx_type);
    unsigned int loadTextureFile(const std::string& filepath, GLenum format, TextureType tx_type);

    CubeMap& loadCubeMap(const std::string& filepath, TextureType tx_type);
    unsigned int loadCubeMapFiles(const std::array<std::string, 6>& filepaths, TextureType tx_type);
    
    Mesh& loadMesh(const std::string& filepath);

    std::tuple<unsigned int, unsigned int, unsigned int> loadOBJFile(const std::string& filepath);
    std::pair<unsigned int, unsigned int> loadToVAO(std::vector<float>& data, unsigned int dim);
    std::pair<unsigned int, unsigned int> loadToVAO(std::vector<float>& pos, std::vector<float>& textCoords, 
                                                     std::vector<float>& normals, std::vector<unsigned int>& indices);


private:
    AssetManager(){}
    unsigned int genAssetID();

    unsigned int generateVAO();
    void storeDataInAttribList(unsigned int attribNum, unsigned int components, std::vector<float> data);

    void processFace(int faceVertices[3][3], std::vector<unsigned int>& indices, std::vector<glm::vec2>& textures, 
                    std::vector<glm::vec3> &normals, std::vector<float> &texturesData, std::vector<float>& normalsData);
    
    std::map<unsigned int, std::unique_ptr<Texture>>* getTextureMap(); 
    std::map<unsigned int, std::unique_ptr<Shader>>* getShaderMap();

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
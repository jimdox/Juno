#pragma once
#include "pch.h"
#include <stb_image.h>
#include<GL/glew.h>
#include "entity/Entity.h"

/* using singleton schenanagans to control memory management
   in one instance across the program */
namespace juno
{
class AssetManager
{
public:
    static AssetManager& getInstance()
    {   
        static AssetManager s_instance;
        return s_instance;
    }

    AssetManager(const AssetManager& assetManager) = delete;

    std::tuple<unsigned int, unsigned int> loadToVAO(std::vector<float>& data, unsigned int dim);
    std::tuple<unsigned int, unsigned int> loadToVAO(std::vector<float>& pos, std::vector<float>& textCoords, 
                                                     std::vector<float>& normals, std::vector<unsigned int>& indices);

    std::tuple<GLuint, GLuint> loadShader(const std::string& filepath);

    unsigned int loadTexture(const std::string& filepath, GLenum format, TextureType tx_type);
    unsigned int loadCubeMap(const std::array<std::string, 6>& filepaths, TextureType tx_type);
    
    const Mesh loadOBJFile(const std::string& filepath);

private:
    AssetManager(){}

    unsigned int generateVAO();
    void storeDataInAttribList(unsigned int attribNum, unsigned int components, std::vector<float> data);

    void processFace(int faceVertices[3][3], std::vector<unsigned int>& indices, std::vector<glm::vec2>& textures, 
                    std::vector<glm::vec3> &normals, std::vector<float> &texturesData, std::vector<float>& normalsData);


    //unsigned int loadModel() -- todo: adaptive loading of different types by parsing file extension
};

}
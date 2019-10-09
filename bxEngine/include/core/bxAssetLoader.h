#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <string>
#include <map>
#include <vector>
#include <istream>
#include <fstream>
#include <sstream>
#include <iostream>
#include "entity/Entity.h"
#include "core/Log.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdlib.h>


/* */
namespace bxImport {

    static std::vector<bbx::Texture> assimp_loadMatTextures(aiMaterial *mat, aiTextureType type, std::string typeStr, std::string& dir)
    {
        std::vector<bbx::Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {

            aiString str;
            mat->GetTexture(type, i, &str);
            std::string texPath = dir + "/" + (str).C_Str();
            bbx::Texture tex(texPath, typeStr);
    
            textures.push_back(tex);
        }
        return textures;
    }


    static bbx::Mesh assimp_processMesh(aiMesh *mesh, const aiScene *scene, std::string& directory)
    {
        std::vector<bbx::Vertex> vertices;
        std::vector<unsigned int> indices;
        bbx::TextureList textures;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            bbx::Vertex vertex;
            glm::vec3 pos;
            glm::vec3 norm;
            glm::vec2 tex(0.0f, 0.0f);
 
            pos.x = mesh->mVertices[i].x;
            norm.x = mesh->mNormals[i].x;
            pos.y = mesh->mVertices[i].y;
            norm.y = mesh->mNormals[i].y;
            pos.z = mesh->mVertices[i].z;
            norm.z = mesh->mVertices[i].z;
            
            vertex.position = pos;
            vertex.normal = norm;

            if(!mesh->mTextureCoords[0])
            {
                BBX_CLI_WARN("Imported object does not have texture coordinates.");
            }

            tex.x = mesh->mTextureCoords[0][i].x;
            tex.y = mesh->mTextureCoords[0][i].y;
            vertex.textureCoord = tex;
            vertices.push_back(vertex);
            
        }
        
        for(unsigned int j = 0; j < mesh->mNumFaces; j++)
        {
            aiFace face = mesh->mFaces[j];
            for(unsigned int k = 0; k < face.mNumIndices; k++)
            {
                indices.push_back(face.mIndices[k]);
            }
        }


        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            textures.diffuse = assimp_loadMatTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
            textures.specular = assimp_loadMatTextures(material, aiTextureType_SPECULAR, "texture_specular", directory);

        }
        return bbx::Mesh(vertices, indices, textures);
    }

    static void assimp_processNode(aiNode *node, const aiScene *scene, std::string& directory, std::vector<bbx::Mesh> &meshes)
    {
        for(unsigned int i = 0; i <node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(assimp_processMesh(mesh, scene, directory));
        }   
        /* repeat for descendent meshes */
        for(unsigned int j = 0; j < node->mNumChildren; j++)
        {
            assimp_processNode(node->mChildren[j], scene, directory,  meshes);
        }
    }


    static void assimp_loadModel(std::string& filepath, std::vector<bbx::Mesh>& meshList)
    {
        Assimp::Importer aimport;
        const aiScene *scene = aimport.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
        {
            BBX_CLI_ERR(("Error loading model: " + filepath).c_str());
            return;
        }
        std::string directory = filepath.substr(0, filepath.find_last_of("/"));
        assimp_processNode(scene->mRootNode, scene, directory, meshList);
    }

    /* ------------------------------------ */

    /* custom obj importer */
    static void loadOBJ(std::string& filepath)
    {
        std::string line;                            /* per line .obj info */
        std::ifstream ioStream(filepath.c_str());
        if(!ioStream.is_open())
        {
            BBX_CLI_ERR(("Error loading file: " + filepath).c_str());
        }
        std::vector<bbx::Vertex> vertexData;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textureCoords;
        std::vector<unsigned int> indices;

        std::vector<std::string> lineData;
        
        while(!ioStream.eof())
        {
            std::getline(ioStream, line);
            std::stringstream ss(line);
            std::string token;
            while(std::getline(ss, token, ' '))
            {
                lineData.push_back(token);          /* split line by ' '  */
            }

            if(lineData[0] == "v")
            {
                glm::vec3 vert(atof(lineData[1].c_str()), atof(lineData[2].c_str()), atof(lineData[3].c_str()));
                vertices.push_back(vert);
            }
            else if (lineData[0] == "vt")
            {
                glm::vec2 texCoord(atof(lineData[1].c_str()), atof(lineData[2].c_str()));
                textureCoords.push_back(texCoord);
            }
            else if(lineData[0] == "vn")
            {
                glm::vec3 norm(atof(lineData[1].c_str()), atof(lineData[2].c_str()), atof(lineData[3].c_str()));
                normals.push_back(norm);
            }
            else if(lineData[0] == "f")
            {
                std::string sV3 = lineData[1].substr(lineData[1].find_last_of("/")+1, lineData[1].length());
                std::string sV1 = lineData[1].substr(0, lineData[1].find_first_of("/"));
                std::string sV2 = lineData[1].substr(sV1.length(), lineData[1].length() - sV3.length());
                BBX_CLI_INFO(sV2);
                                
            }
            lineData.clear();
        }
  
    }

    
   
	
}

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

    // static std::vector<bbx::Texture> assimp_loadMatTextures(aiMaterial *mat, aiTextureType type, std::string typeStr, std::string& dir)
    // {
    //     std::vector<bbx::Texture> textures;
    //     for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    //     {

    //         aiString str;
    //         mat->GetTexture(type, i, &str);
    //         std::string texPath = dir + "/" + (str).C_Str();
    //         bbx::Texture tex(texPath, typeStr);
    
    //         textures.push_back(tex);
    //     }
    //     return textures;
    // }


    // static bbx::Mesh assimp_processMesh(aiMesh *mesh, const aiScene *scene, std::string& directory)
    // {
    //     std::vector<bbx::Vertex> vertices;
    //     std::vector<unsigned int> indices;
    //     bbx::TextureList textures;

    //     for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    //     {
    //         bbx::Vertex vertex;
    //         glm::vec3 pos;
    //         glm::vec3 norm;
    //         glm::vec2 tex(0.0f, 0.0f);
 
    //         pos.x = mesh->mVertices[i].x;
    //         norm.x = mesh->mNormals[i].x;
    //         pos.y = mesh->mVertices[i].y;
    //         norm.y = mesh->mNormals[i].y;
    //         pos.z = mesh->mVertices[i].z;
    //         norm.z = mesh->mVertices[i].z;
            
    //         vertex.position = pos;
    //         vertex.normal = norm;

    //         if(!mesh->mTextureCoords[0])
    //         {
    //             BBX_CLI_WARN("Imported object does not have texture coordinates.");
    //         }

    //         tex.x = mesh->mTextureCoords[0][i].x;
    //         tex.y = mesh->mTextureCoords[0][i].y;
    //         vertex.textureCoord = tex;
    //         vertices.push_back(vertex);
            
    //     }
        
    //     for(unsigned int j = 0; j < mesh->mNumFaces; j++)
    //     {
    //         aiFace face = mesh->mFaces[j];
    //         for(unsigned int k = 0; k < face.mNumIndices; k++)
    //         {
    //             indices.push_back(face.mIndices[k]);
    //         }
    //     }


    //     if(mesh->mMaterialIndex >= 0)
    //     {
    //         aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    //         textures.diffuse = assimp_loadMatTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
    //         textures.specular = assimp_loadMatTextures(material, aiTextureType_SPECULAR, "texture_specular", directory);

    //     }
    //     //return bbx::Mesh(vertices, indices, textures);
    // }

    // static void assimp_processNode(aiNode *node, const aiScene *scene, std::string& directory, std::vector<bbx::Mesh> &meshes)
    // {
    //     for(unsigned int i = 0; i <node->mNumMeshes; i++)
    //     {
    //         aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    //         meshes.push_back(assimp_processMesh(mesh, scene, directory));
    //     }   
    //     /* repeat for descendent meshes */
    //     for(unsigned int j = 0; j < node->mNumChildren; j++)
    //     {
    //         assimp_processNode(node->mChildren[j], scene, directory,  meshes);
    //     }
    // }


    // static void assimp_loadModel(std::string& filepath, std::vector<bbx::Mesh>& meshList)
    // {
    //     Assimp::Importer aimport;
    //     const aiScene *scene = aimport.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
    //     if(!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    //     {
    //         BBX_CLI_ERR(("Error loading model: " + filepath).c_str());
    //         return;
    //     }
    //     std::string directory = filepath.substr(0, filepath.find_last_of("/"));
    //     assimp_processNode(scene->mRootNode, scene, directory, meshList);
    // }

    /* ------------------------------------ */


    static void processFace(int faceVertices[3][3], std::vector<unsigned int> &indices, std::vector<glm::vec2> &textures, 
                                std::vector<glm::vec3> &normals, std::vector<float> &texturesData, std::vector<float> &normalsData )
    {
        for(int i = 0; i < 3; i++)
        {
            int currentVertPtr = faceVertices[i][0] - 1;
            indices.push_back(currentVertPtr);

            glm::vec2 currentTexCoord = textures[faceVertices[i][1]-1];
            texturesData[currentVertPtr*2] = currentTexCoord.x;
            texturesData[currentVertPtr*2 +1] = 1 - currentTexCoord.y;



            glm::vec3 currentNormal = normals[faceVertices[i][2]-1];
            normalsData[currentVertPtr * 3] = currentNormal.x;
            normalsData[currentVertPtr * 3 +1] = currentNormal.y;
            normalsData[currentVertPtr * 3 +2] = currentNormal.z;
        }
    }



    /* custom obj importer */
    static bbx::Mesh* loadOBJ(std::string& filepath)
    {
        std::string line;                            /* per line .obj info */
        std::ifstream openFile(filepath.c_str());
        if(!openFile.is_open())
        {
            BBX_CLI_ERR(("Error loading file: " + filepath).c_str());
        }
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textures;
        std::vector<unsigned int> indices;
        
        std::vector<float> verticesData;
        std::vector<float> texturesData;
        std::vector<float> normalsData;


        std::vector<std::string> lineData;
        
        while(!openFile.eof())
        {
            std::getline(openFile, line);
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
                textures.push_back(texCoord);
            }
            else if(lineData[0] == "vn")
            {
                glm::vec3 norm(atof(lineData[1].c_str()), atof(lineData[2].c_str()), atof(lineData[3].c_str()));
                normals.push_back(norm);
            }
            if(lineData[0] == "f"){ break; }
            lineData.clear();
        }

        verticesData.reserve(vertices.size() * 3);
        normalsData.reserve(vertices.size() * 3);
        texturesData.reserve(vertices.size() * 2);

        for(unsigned int i = 0; i < vertices.size() * 2; i++)
        {
            texturesData.push_back(0.0f);
        }

        for(unsigned int i = 0; i < vertices.size() * 3; i++)
        {
            normalsData.push_back(0.0f);
        }

        lineData.clear();

        while(!openFile.eof())    
        {
            std::getline(openFile, line);
            std::stringstream ss(line);
            std::string token;
            while(std::getline(ss, token, ' '))
            {
                lineData.push_back(token);          /* split line by ' '  */
            }

            if(lineData[0] == "f")
            {
            
                int faceVertices[3][3];

                for(unsigned int i = 0; i < 3; i++)
                {
                    std::stringstream ssVert(lineData[i+1]);
                    std::string tokVert;

                    for(int j = 0; j < 3; j++)
                    {
                        std::getline(ssVert, tokVert, '/');
                        faceVertices[i][j] = atoi(tokVert.c_str());
                    }

                                    

                }
                processFace(faceVertices, indices, textures, normals, texturesData, normalsData);

            }
            lineData.clear();
        }
        openFile.close();
        /* done parsing obj */


        int vPtr = 0;
        for(glm::vec3 v : vertices)
        {
            verticesData[vPtr++] = v.x;
            verticesData[vPtr++] = v.y;
            verticesData[vPtr++] = v.z;
        }

        return new bbx::Mesh(verticesData, texturesData, normalsData, indices);
    }

    
   
	
}
